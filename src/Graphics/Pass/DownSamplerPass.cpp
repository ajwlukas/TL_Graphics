#include "pch_dx_11.h"
#include "DownSamplerPass.h"

#include "Pipeline.h"

DownSamplerPass::DownSamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();
}

DownSamplerPass::~DownSamplerPass()
{
	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void DownSamplerPass::Set()
{
	rtt->SetRT(0);
	shaderPS->Set();
}

void DownSamplerPass::Execute()
{
	Set();
	pipeline->Draw();
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);
}

void DownSamplerPass::ClearRenderTargets()
{
	rtt->Clear();
}

void DownSamplerPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 0.5f, 0.5f);
}

void DownSamplerPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SamplerPS.hlsl");
}
