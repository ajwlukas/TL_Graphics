#include "pch_dx_11.h"
#include "DeferredRenderPass.h"

#include "Pipeline.h"

DeferredRenderPass::DeferredRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();
}

DeferredRenderPass::~DeferredRenderPass()
{
	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void DeferredRenderPass::Set()
{
	rtt->SetRT(0);
	shaderPS->Set();
}

void DeferredRenderPass::Execute()
{
	ClearRenderTargets();
	Set();
	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);
}

void DeferredRenderPass::ClearRenderTargets()
{
	rtt->Clear();
}

void DeferredRenderPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice);
}

void DeferredRenderPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_DeferredPS.hlsl");
}
