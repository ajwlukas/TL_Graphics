#include "pch_dx_11.h"
#include "Shadow.h"

Shadow::Shadow(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, Camera* camera, UINT directionalShadowNum)
	:directionalShadowNum(directionalShadowNum)
{
	CalculateSize(camera);
	CreateRTTs(resizeNotice);
}

Shadow::~Shadow()
{
}

void Shadow::CalculateSize(Camera* camera)
{
	TL_Math::Vector3 frustumMiddle = camera->data.camPos + TL_Math::Vector3(XMVector3Transform(TL_Math::Vector3(0,0,1), camera->data.viewInv) * (camera->frustumFar + camera->frustumNear) * 0.5f);//frustum middle spot

	TL_Math::Vector3 middlePoint = { 0,0,0 };

	//ºûÀÇ ¿ª¹æÇâÀ» ÃÄ´Ù º¸°í ÀÖ´Â ÁÂÇ¥°è¸¦ ±¸ÇÔ
	dir.Normalize();

	TL_Math::Vector3 axisZ = -dir;
	TL_Math::Vector3 axisX = axisZ.Cross(TL_Math::Vector3(0,1,0));
	TL_Math::Vector3 axisY = axisX.Cross(axisZ);


	float max = FLT_MIN;
	float min = FLT_MAX;

	float axisScalar = 0.0f;

	{//ºû ÁÂÇ¥°è xÃàÀ¸·Î Åõ¿µ
		axisScalar = axisX.Dot(camera->worldPoints.LTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(camera->worldPoints.LBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(camera->worldPoints.RTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(camera->worldPoints.RBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(camera->worldPoints.LTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(camera->worldPoints.LBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(camera->worldPoints.RTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(camera->worldPoints.RBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		width = max - min;

		TL_Math::Vector3 temp = XMVectorScale(axisX, (max + min) * 0.5f);

		middlePoint += temp;
	}

	{//ºû ÁÂÇ¥°è yÃàÀ¸·Î Åõ¿µ
		axisScalar = axisY.Dot(camera->worldPoints.LTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(camera->worldPoints.LBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(camera->worldPoints.RTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(camera->worldPoints.RBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(camera->worldPoints.LTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(camera->worldPoints.LBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(camera->worldPoints.RTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(camera->worldPoints.RBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		height = max - min;

		TL_Math::Vector3 temp = XMVectorScale(axisY, (max + min) * 0.5f);

		middlePoint += temp;
	}

	{//ºû ÁÂÇ¥°è zÃàÀ¸·Î Åõ¿µ
		axisScalar = axisZ.Dot(camera->worldPoints.LTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(camera->worldPoints.LBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(camera->worldPoints.RTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(camera->worldPoints.RBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(camera->worldPoints.LTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(camera->worldPoints.LBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(camera->worldPoints.RTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(camera->worldPoints.RBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		depth = max - min;

		//frustumÀÇ ¿Ü°ûÀ¸·Î °¡°Ô²û
		TL_Math::Vector3 temp = XMVectorScale(axisZ, max);

		middlePoint += temp;
	}

	lightTransformR = TL_Math::Matrix(axisX, axisY, axisZ);
	lightTransformR.m[3][0] = middlePoint.x;
	lightTransformR.m[3][1] = middlePoint.y;
	lightTransformR.m[3][2] = middlePoint.z;

	
	lightR.camPos = middlePoint;
	lightR.proj = XMMatrixOrthographicLH(width, height, 1.0f, 2000.0f);
	lightR.projInv = XMMatrixInverse(nullptr, lightR.proj);
	lightR.viewInv = lightTransformR;
	lightR.view = XMMatrixInverse(nullptr, lightR.viewInv);








	//ºûÀÌ ÇâÇÏ´Â ¹æÇâÀ» ÃÄ´Ùº¸°í ÀÖ´Â ÁÂÇ¥°è
	axisZ = dir;
	axisX = dir.Cross(TL_Math::Vector3(0,1,0));
	axisY = axisX.Cross(axisZ);

	lightTransform = TL_Math::Matrix(axisX, axisY, axisZ);
}

void Shadow::CreateRTTs(OnResizeNotice* resizeNotice)
{
	shadowMapDirectionalLight.resize(directionalShadowNum);

	for (auto s : shadowMapDirectionalLight)
	{
		s = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, (UINT)1000, (UINT)1000, "ShadowMap");
	}
}

void Shadow::Execute()
{
}

void Shadow::CreateDepthStateAndView()
{
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};

		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_GREATER;

		desc.StencilEnable = true;
		desc.StencilReadMask = 0xFF;
		desc.StencilWriteMask = 0xFF;

		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		resources->depthStencilStates->Get(depthState, desc);
	}


	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.AntialiasedLineEnable = false;
		desc.CullMode = D3D11_CULL_NONE;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.FrontCounterClockwise = true;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;

		resources->rasterStates->Get(rasterState, desc);
	}

	resources->depthStencilViews->CreateDefault(depthView, nullptr);
}
