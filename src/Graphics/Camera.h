#pragma once

#include "ICamera.h"

#include "OnResize.h"

#include "Math\TL_Math.h"

#include "ConstantBuffer.h"

class Camera : public TL_Graphics::ICamera, public OnResize_Observer
{
public:
	Camera(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float fov = 80.0f, float frustumNear = 1.0f, float frustumFar = 2000.0f);
	~Camera();

	virtual void Set(TL_Graphics::E_SHADER_TYPE type = TL_Graphics::E_SHADER_TYPE::VS,
		UINT slot = 0) override;
	virtual void Update(TL_Math::Vector3 pos, TL_Math::Vector3 rot) override;
	virtual void Update(TL_Math::Vector3 pos, TL_Math::Quaternion rot) override;
	virtual void Update(TL_Math::Matrix m) override;

private:

	__declspec(align(16)) struct Data
	{
		TL_Math::Matrix view;
		TL_Math::Matrix  proj;
		TL_Math::Matrix  viewInv;
		TL_Math::Matrix  projInv;
		TL_Math::Vector3 camPos;
	}data;

	ConstantBuffer* viewprojBuffer;

	float fov;//시야각
	float fovInRadian;
	float frustumNear;
	float frustumFar;

	//보류
	virtual void  OnResize(UINT width, UINT height) override;
};