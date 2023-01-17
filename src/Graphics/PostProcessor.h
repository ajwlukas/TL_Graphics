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
/// ��ó�� ���
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

	GridPass* gridPass;//��ó���� �ش���� ����, screenMesh Ȱ���ϱ� ���ؼ� ���� ����

	ScreenMesh* screenMesh;

	DeferredRenderPass* deferredRenderPass;

	DownSamplerPass* downSamplerPass;

	ColorGradingPass* colorGradingPass;

	GaussianBlurPass* gaussianBlurPassX;
	GaussianBlurPass* gaussianBlurPassY;

	FinalPass* finalPass;

};