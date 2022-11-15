#include "pch_dx_11.h"
#include "Camera.h"

constexpr float pi  = 3.141592f;

Camera::Camera(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float fov, float frustumNear, float frustumFar)
	:data{}
	,fov(fov)
	,frustumNear(frustumNear)
	,frustumFar(frustumFar)
{
	fovInRadian = pi / 180.0f * fov;
	
	resizeNotice->AddObserver(this);

	data.view = XMMatrixInverse(nullptr, XMMatrixIdentity());
	data.proj = XMMatrixPerspectiveFovLH(fovInRadian, resizeNotice->GetWidth() / (float)resizeNotice->GetHeight(), frustumNear, frustumFar);

	data.camPos = { 0,0,0 };

	viewprojBuffer = new ConstantBuffer(dc, resources, pipeline,  &data, sizeof(Data));
}

Camera::~Camera()
{
	SAFE_DELETE(viewprojBuffer);
}

void Camera::Set(TL_Graphics::E_SHADER_TYPE type,
	UINT slot)
{
	viewprojBuffer->Set(type, slot);
}

void Camera::Update(TL_Math::Vector3 pos, TL_Math::Vector3 rot)
{
	TL_Math::Matrix R = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
	TL_Math::Matrix T = DirectX::XMMatrixTranslationFromVector(pos);

	TL_Math::Matrix m = XMMatrixIdentity();

	m *= R;
	m *= T;

	data.view = XMMatrixInverse(nullptr, m);

	data.camPos = pos;

	viewprojBuffer->Update(&data, sizeof(Data));
}

void Camera::Update(TL_Math::Vector3 pos, TL_Math::Quaternion rot)
{
	TL_Math::Matrix R = DirectX::XMMatrixRotationQuaternion(rot);
	TL_Math::Matrix T = DirectX::XMMatrixTranslationFromVector(pos);

	TL_Math::Matrix m = XMMatrixIdentity();

	m *= R;
	m *= T;

	data.view = XMMatrixInverse(nullptr, m);

	data.camPos = pos;

	viewprojBuffer->Update(&data, sizeof(Data));
}

void Camera::Update(TL_Math::Matrix m)
{
	data.view = XMMatrixInverse(nullptr, m);

	XMVECTOR p, r, s;

	XMMatrixDecompose(&s, &r, &p, m);

	XMStoreFloat3(&data.camPos, p);

	viewprojBuffer->Update(&data, sizeof(Data));
}


void Camera::OnResize(UINT width, UINT height)
{
	data.proj = XMMatrixPerspectiveFovLH(fovInRadian, width / (float)height, frustumNear, frustumFar);

	viewprojBuffer->Update(&data, sizeof(Data));
}

