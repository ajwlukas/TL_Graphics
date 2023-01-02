#include "pch_dx_11.h"
#include "GBufferRenderPass.h"

#include "Pipeline.h"

GBufferRenderPass::GBufferRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	: 
	IRenderPass(dc, resources, pipeline)
{
	CreateRenderTargets(resizeNotice);
	CreateBlendStates();
	CreateShader();

}

GBufferRenderPass::~GBufferRenderPass()
{

	for (UINT i = 0; i < 8; i++)
	{
		delete rtts[i];
	}

	delete shaderPS;
}


void GBufferRenderPass::Set()
{
	for (UINT i = 0; i < 8; i++)
	{
		rtts[i]->SetRT(i);
	}

	shaderPS->Set();

	pipeline->SetCurrentBlendState(blendState);
}

void GBufferRenderPass::CreateRenderTargets(OnResizeNotice* resizeNotice)
{
	for (UINT i = 0; i < 8; i++)
	{
		rtts[i] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice);
	}
}

void GBufferRenderPass::CreateBlendStates()
{
	D3D11_BLEND_DESC desc = {};
	for (UINT i = 0; i < 8; i++)
	{
		desc.RenderTarget[i].BlendEnable = FALSE;
		desc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[i].RenderTargetWriteMask = 0x0f;
	}

	resources->blendStates->Get(blendState, desc);

}

void GBufferRenderPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_G_Buffers.hlsl");
}
