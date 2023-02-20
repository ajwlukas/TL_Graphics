#include "pch_dx_11.h"
#include "IRenderPass.h"

IRenderPass::IRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT rttNum, UINT sourceTextureNum)
	: dc(dc)
	, resources(resources)
	, pipeline(pipeline)
	, resizeNotice(resizeNotice)
{
	rtts.resize(rttNum);
	sourceTextures.resize(sourceTextureNum);
	DescViewPort();
	ResizeViewport();
}

IRenderPass::~IRenderPass()
{
}

void IRenderPass::SetSourceTexture(Texture* texture, UINT sourceTextureNum)
{
	sourceTextures[sourceTextureNum] = texture;
}

Texture* IRenderPass::GetDestTexture(UINT destTextureNum)
{
	return rtts[destTextureNum];
}

void IRenderPass::SetRatio(float sizeX, float sizeY)
{
	isBasedWindowSize = true;
	widthRatio = sizeX;
	heightRatio = sizeY;


	ResizeViewport();
	ResizeRTTs();
}

void IRenderPass::SetSize(UINT sizeX, UINT sizeY)
{
	isBasedWindowSize = false;
	width = sizeX;
	height = sizeY;

	ResizeViewport();
	ResizeRTTs();
}

void IRenderPass::DescViewPort()
{
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
}

void IRenderPass::ResizeViewport()
{
	if (isBasedWindowSize)
	{
		assert(widthRatio > 0.0f);//assert When value is not set
		assert(heightRatio > 0.0f);

		viewPort.Width = resizeNotice->GetWidth() * widthRatio;
		viewPort.Height = resizeNotice->GetHeight() * heightRatio;
	}
	else
	{
		assert(width > 0);
		assert(height > 0);

		viewPort.Width = (float)width;
		viewPort.Height = (float)height;
	}
}

void IRenderPass::ResizeRTTs()
{
	if (isBasedWindowSize)
	{
		assert(widthRatio > 0.0f);//assert When value is not set
		assert(heightRatio > 0.0f);

		for (auto rtt : rtts)
			rtt->ResizeRatio(widthRatio, heightRatio);
	}
	else
	{
		assert(width > 0);
		assert(height > 0);

		for (auto rtt : rtts)
			rtt->Resize(width, height);
	}
}
