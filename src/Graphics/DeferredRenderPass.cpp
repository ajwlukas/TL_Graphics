#include "pch_dx_11.h"
#include "DeferredRenderPass.h"

#include "Pipeline.h"

DeferredRenderPass::DeferredRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
{
}

DeferredRenderPass::~DeferredRenderPass()
{
	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void DeferredRenderPass::Set()
{
	pipeline->SetSwapChainRenderTargetView();
	rtt->SetRT(0);
	shaderPS->Set();
}

void DeferredRenderPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice);
}

void DeferredRenderPass::CreateShader()
{

	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_DeferredPS.hlsl");
}
