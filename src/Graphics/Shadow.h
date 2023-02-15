#pragma once

#include "Shadow.h"

#include "Resources.h"

#include "Pipeline.h"

#include "OnResize.h"

#include "Camera.h"

#include "RenderTargetTexture.h"

const float max_depth = 8000.0f;

const UINT rtSizeHigh = 10000;
const UINT rtSizeMid = 5000;
const UINT rtSizeLow = 1000;

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

	void CalculateSize(TL_Math::Vector3 LTN, TL_Math::Vector3 RTN, TL_Math::Vector3 LBN, TL_Math::Vector3 RBN,
		TL_Math::Vector3 LTF, TL_Math::Vector3 RTF, TL_Math::Vector3 LBF, TL_Math::Vector3 RBF, TL_Math::Vector3& middlePoint, float& width, float& height);
	void CalculateSizeNew();
	float size = 0;
	
	TL_Math::Matrix lightTransformHigh;
	TL_Math::Matrix lightTransformMid;
	TL_Math::Matrix lightTransformLow;

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
		float frustumFar;
	}lightCamHigh, lightCamMid, lightCamLow;//0 = high, 1 = mid, 2 = low

	Resource<ID3D11DepthStencilState> depthState;

	Resource<ID3D11Texture2D> depthStencilBufferHigh;
	Resource<ID3D11Texture2D> depthStencilBufferMid;
	Resource<ID3D11Texture2D> depthStencilBufferLow;

	Resource<ID3D11DepthStencilView> depthStencilViewHigh;
	Resource<ID3D11DepthStencilView> depthStencilViewMid;
	Resource<ID3D11DepthStencilView> depthStencilViewLow;

	Resource<ID3D11RasterizerState> rasterState;

	void CreateDepthStateAndView();

	void DescViewport(float size);

	D3D11_VIEWPORT viewPort;

	ConstantBuffer* lightSpaceViewProj;
	RenderTargetTexture* depthFromLightHigh;
	RenderTargetTexture* depthFromLightMid;
	RenderTargetTexture* depthFromLightLow;

	Resource<ID3D11SamplerState> samplerState;

	Camera* camera;


	Shader* shadowShader;//빛을 기준으로 깊이 구해주는 쉐이더
	void CreateShader();
	void CreateAndSetSamplerState();
	void CreateRasterState();


	TL_Math::Vector3 axisZ;
	TL_Math::Vector3 axisX;
	TL_Math::Vector3 axisY;
};