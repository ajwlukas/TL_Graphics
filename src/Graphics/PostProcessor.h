#pragma once

#include "ControlPanel.h"

#include "ScreenMesh.h"

#include "Pass/GBufferRenderPass.h"

#include "Pass/DeferredRenderPass.h"

#include "Pass/GridPass.h"

#include "Pass/DownSamplerPass.h"

#include "Pass/ColorGradingPass.h"

#include "Pass/GaussianBlurPass.h"

#include "Pass/FinalPass.h"

/// <summary>
/// 후처리 담당
/// </summary>

class PostProcessor
{
public:
	PostProcessor(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~PostProcessor();
	
	void Execute();

	TL_Graphics::ControlPanel control;

private:
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	GridPass* gridPass;//후처리에 해당되지 않음, screenMesh 활용하기 위해서 같이 있음

	ScreenMesh* screenMesh;

	DeferredRenderPass* deferredRenderPass;

	DownSamplerPass* downSamplerPass;

	ColorGradingPass* colorGradingPass;

	GaussianBlurPass* gaussianBlurPassX;
	GaussianBlurPass* gaussianBlurPassY;

	FinalPass* finalPass;

};