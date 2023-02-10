#pragma once

#include "Shadow.h"

#include "Resources.h"

#include "Pipeline.h"

#include "OnResize.h"

#include "Camera.h"

#include "RenderTargetTexture.h"

const float max_depth = 4000.0f;

class Shadow
{
public:
	Shadow(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice,Camera* camera, UINT directionalShadowNum);
	~Shadow();



	void Execute();
private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;


	UINT directionalShadowNum;

	void CalculateSize();
	float size = 0;
	
	TL_Math::Matrix lightTransform;

	TL_Math::Vector3 dir = { 0,0,1 };//일단 빛 생각하지 말아보자

	float width = 0;
	float height = 0;
	float depth = 0;

	void ClearRTTs();
	void CreateRTTs(OnResizeNotice* resizeNotice);


	__declspec(align(16)) struct Data
	{
		TL_Math::Matrix view;
		TL_Math::Matrix  proj;
		TL_Math::Matrix  viewInv;
		TL_Math::Matrix  projInv;
		TL_Math::Vector3 camPos;
	}lightCam;

	Resource<ID3D11DepthStencilState> depthState;

	Resource<ID3D11Texture2D> depthStencilBuffer;
	Resource<ID3D11DepthStencilView> depthStencilView;

	Resource<ID3D11RasterizerState> rasterState;

	void CreateDepthStateAndView();

	void DescViewport();

	D3D11_VIEWPORT viewPort;

	ConstantBuffer* lightSpaceViewProj;
	RenderTargetTexture* depthFromLight;

	Camera* camera;

	Shader* shadowShader;//빛을 기준으로 깊이 구해주는 쉐이더
	void CreateShader();
};