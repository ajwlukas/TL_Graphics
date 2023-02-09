#pragma once

#include "Shadow.h"

#include "Resources.h"

#include "Pipeline.h"

#include "OnResize.h"

#include "Camera.h"

#include "RenderTargetTexture.h"

class Shadow
{
public:
	Shadow(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice,Camera* camera, UINT directionalShadowNum);
	~Shadow();

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;


	UINT directionalShadowNum;
	vector<RenderTargetTexture*> shadowMapDirectionalLight;

	void CalculateSize(Camera* camera);
	float size = 0;
	
	TL_Math::Matrix lightTransform;
	TL_Math::Matrix lightTransformR;//R = reverse

	TL_Math::Vector3 dir = { 1,1,1 };//일단 빛 생각하지 말아보자

	float width = 0;
	float height = 0;
	float depth = 0;

	void CreateRTTs(OnResizeNotice* resizeNotice);

	void Execute();

	__declspec(align(16)) struct Data
	{
		TL_Math::Matrix view;
		TL_Math::Matrix  proj;
		TL_Math::Matrix  viewInv;
		TL_Math::Matrix  projInv;
		TL_Math::Vector3 camPos;
	}lightR;

	Resource<ID3D11DepthStencilState> depthState;
	Resource<ID3D11DepthStencilView> depthView;

	Resource<ID3D11RasterizerState> rasterState;

	void CreateDepthStateAndView();

	ConstantBuffer* lightSpaceViewProj;
};