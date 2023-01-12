#pragma once

#include "ControlPanel.h"

#include "ScreenMesh.h"

#include "Pass/GBufferRenderPass.h"

#include "Pass/DeferredRenderPass.h"

#include "Pass/GridPass.h"

#include "Pass/DownSamplerPass.h"

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

private:
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	ControlPanel control;

	GridPass* gridPass;//��ó���� �ش���� ����, screenMesh Ȱ���ϱ� ���ؼ� ���� ����

	ScreenMesh* screenMesh;

	DeferredRenderPass* deferredRenderPass;

	DownSamplerPass* downSamplerPass;

	FinalPass* finalPass;

};