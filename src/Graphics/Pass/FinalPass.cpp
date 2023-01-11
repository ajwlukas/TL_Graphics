#include "pch_dx_11.h"
#include "FinalPass.h"

#include "Pipeline.h"

FinalPass::FinalPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
{
	CreateShader();
}

FinalPass::~FinalPass()
{
	SAFE_DELETE(shaderPS);
}

void FinalPass::Set()
{
	pipeline->SetSwapChainRenderTargetView(0);
	shaderPS->Set();
}

void FinalPass::Execute()
{
	Set();
	pipeline->Draw();
}

void FinalPass::ClearRenderTargets()
{
}

void FinalPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SamplerPS.hlsl");
}
