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

	aspectRatio = resizeNotice->GetWidth() / (float)resizeNotice->GetHeight();

	data.view = XMMatrixInverse(nullptr, XMMatrixIdentity());
	data.proj = XMMatrixPerspectiveFovLH(fovInRadian, aspectRatio, frustumNear, frustumFar);
	data.viewInv = XMMatrixInverse(nullptr, data.view);
	data.projInv = XMMatrixInverse(nullptr, data.proj);


	data.camPos = { 0,0,0 };

	viewprojBuffer = new ConstantBuffer(dc, resources, pipeline,  &data, sizeof(Data),"Camera");

	CalculatePoints();
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
	data.viewInv = XMMatrixInverse(nullptr, data.view);

	data.camPos = pos;

	viewprojBuffer->Update(&data, sizeof(Data));

	CalculateWorldPoints();
}

void Camera::Update(TL_Math::Vector3 pos, TL_Math::Quaternion rot)
{
	TL_Math::Matrix R = DirectX::XMMatrixRotationQuaternion(rot);
	TL_Math::Matrix T = DirectX::XMMatrixTranslationFromVector(pos);

	TL_Math::Matrix m = XMMatrixIdentity();

	m *= R;
	m *= T;

	data.view = XMMatrixInverse(nullptr, m);
	data.viewInv = XMMatrixInverse(nullptr, data.view);

	data.camPos = pos;

	viewprojBuffer->Update(&data, sizeof(Data));

	CalculateWorldPoints();
}

void Camera::Update(TL_Math::Matrix m)
{
	data.viewInv = m;
	data.view = XMMatrixInverse(nullptr, m);

	XMVECTOR p, r, s;

	XMMatrixDecompose(&s, &r, &p, m);

	XMStoreFloat3(&data.camPos, p);

	viewprojBuffer->Update(&data, sizeof(Data));

	CalculateWorldPoints();
}


void Camera::OnResize(UINT width, UINT height)
{
	aspectRatio = width / (float)height;

	data.proj = XMMatrixPerspectiveFovLH(fovInRadian, aspectRatio, frustumNear, frustumFar);
	data.projInv = XMMatrixInverse(nullptr, data.proj);

	viewprojBuffer->Update(&data, sizeof(Data));
}

void Camera::CalculatePoints()
{
	float farZ = frustumFar;
	float farY = tanf(fovInRadian / 2) * farZ;
	float farX = aspectRatio * farY;

	points.RTF = {	farX,	farY,	farZ };
	points.LTF = {-	farX,	farY,	farZ };
	points.RBF = {	farX,-	farY,	farZ };
	points.LBF = {-	farX,-	farY,	farZ };


	float nearZ = frustumNear;
	float nearY = tanf(fovInRadian / 2) * nearZ;
	float nearX = aspectRatio * nearY;

	points.RTN = {	nearX,	nearY,	nearZ };
	points.LTN = {-	nearX,	nearY,	nearZ };
	points.RBN = {	nearX,-	nearY,	nearZ };
	points.LBN = {-	nearX,-	nearY,	nearZ };
}

void Camera::CalculateWorldPoints()
{
	worldPoints.RTF = XMVector3Transform(points.RTF, data.viewInv);
	worldPoints.LTF = XMVector3Transform(points.LTF, data.viewInv);
	worldPoints.RBF = XMVector3Transform(points.RBF, data.viewInv);
	worldPoints.LBF = XMVector3Transform(points.LBF, data.viewInv);

	worldPoints.RTN = XMVector3Transform(points.RTN, data.viewInv);
	worldPoints.LTN = XMVector3Transform(points.LTN, data.viewInv);
	worldPoints.RBN = XMVector3Transform(points.RBN, data.viewInv);
	worldPoints.LBN = XMVector3Transform(points.LBN, data.viewInv);
}

