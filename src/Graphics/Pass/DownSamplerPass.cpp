#include "pch_dx_11.h"
#include "DownSamplerPass.h"

#include "Pipeline.h"

DownSamplerPass::DownSamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
	,resizeNotice(resizeNotice)
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
	ClearRenderTargets();

	pipeline->ResizeViewPort(resizeNotice->GetWidth() * 0.125f, resizeNotice->GetHeight() * 0.125f);

	Set();
	pipeline->Draw();
	pipeline->UnSetRenderTarget(0);
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);


	pipeline->ResizeViewPort(resizeNotice->GetWidth(), resizeNotice->GetHeight());
}

void DownSamplerPass::ClearRenderTargets()
{
	rtt->Clear();
}

void DownSamplerPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 0.125f, 0.125f);
}

void DownSamplerPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SamplerPS.hlsl");
}
