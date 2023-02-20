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

	DescViewPort();
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
	
	assert(sourceTexture);
	sourceTexture->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);

	ResizeViewport();

	auto oldViewport = pipeline->SetViewPort(&viewPort);

	Set();

	pipeline->Draw();


	//정리
	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}

void SamplerPass::ClearRenderTargets()
{
	rtt->Clear();
}

void SamplerPass::SetSourceTexture(Texture* texture)
{
	assert(rtt != texture);//자기 자신의 텍스쳐는 꽂으면 안됨 ex) t->SetSourceTexture(t->GetDestTexture);
	sourceTexture = texture;
}

void SamplerPass::SetRatio(float sizeX, float sizeY)
{
	isBasedWindowSize = true;
	widthRatio = sizeX;
	heightRatio = sizeY;
}

void SamplerPass::SetSize(UINT sizeX, UINT sizeY)
{
	isBasedWindowSize = false;
	width = sizeX;
	height = sizeY;
}

void SamplerPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	if (isBasedWindowSize)
		rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, widthRatio, heightRatio);
	else //!isBasedWindowSize
		rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, width, height);
}

void SamplerPass::CreateShader()
{
	isShaderCreatedHere = true;
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SamplerPS.hlsl");
}

void SamplerPass::DescViewPort()
{
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
}

void SamplerPass::ResizeViewport()
{
	if (isBasedWindowSize)
	{
		assert(!widthRatio);//assert When value is not set
		assert(!heightRatio);

		viewPort.Width = resizeNotice->GetWidth() * widthRatio;
		viewPort.Height = resizeNotice->GetHeight() * heightRatio;
	}
	else
	{
		assert(!width);
		assert(!height);

		viewPort.Width = (float)width;
		viewPort.Height = (float)height;
	}


	//if (isBasedWindowSize)
	//	pipeline->ResizeViewPort(resizeNotice->GetWidth() * widthRatio, resizeNotice->GetHeight() * heightRatio);
	//else//!isBasedWindowSize
	//	pipeline->ResizeViewPort(width, height);
}
