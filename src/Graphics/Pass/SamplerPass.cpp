#include "pch_dx_11.h"
#include "SamplerPass.h"

#include "Pipeline.h"

SamplerPass::SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float widthRatio, float heightRatio, Shader* shaderPS)
	:IRenderPass(dc, resources, pipeline)
	, resizeNotice(resizeNotice)
	, isBasedWindowSize(true)
	, widthRatio(widthRatio)
	, heightRatio(heightRatio)
	, shaderPS(shaderPS)
{
	CreateRenderTarget(resizeNotice);

	if (!shaderPS)
		CreateShader();
}

SamplerPass::SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT width, UINT height, Shader* shaderPS)
	:IRenderPass(dc, resources, pipeline)
	, resizeNotice(resizeNotice)
	, isBasedWindowSize(false)
	, width(width)
	, height(height)
	, shaderPS(shaderPS)
{
	CreateRenderTarget(resizeNotice);


	if (!shaderPS)
		CreateShader();
}

SamplerPass::~SamplerPass()
{
	SAFE_DELETE(rtt);

	if(isShaderCreatedHere)//스마트 포인터 공부하고 고칠 것
	SAFE_DELETE(shaderPS);
}

void SamplerPass::Set()
{
	rtt->SetRT(0);
	shaderPS->Set();
}

void SamplerPass::Execute()
{
	ClearRenderTargets();


	if (isBasedWindowSize)
		pipeline->ResizeViewPort(resizeNotice->GetWidth() * widthRatio, resizeNotice->GetHeight() * heightRatio);
	else//!isBasedWindowSize
		pipeline->ResizeViewPort(width, height);

	Set();
	pipeline->Draw();
	pipeline->UnSetRenderTarget(0);
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);


	pipeline->ResizeViewPort(resizeNotice->GetWidth(), resizeNotice->GetHeight());
}

void SamplerPass::ClearRenderTargets()
{
	rtt->Clear();
}

void SamplerPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	if (isBasedWindowSize)
		rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, widthRatio, heightRatio, "Sampler");
	else //!isBasedWindowSize
		rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, width, height, "Sampler");
}

void SamplerPass::CreateShader()
{
	isShaderCreatedHere = true;
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SamplerPS.hlsl", "Sampler");
}
