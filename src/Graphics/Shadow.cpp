#include "pch_dx_11.h"
#include "Shadow.h"

Shadow::Shadow(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, Camera* camera, UINT directionalShadowNum)
	:directionalShadowNum(directionalShadowNum)
	, dc(dc)
	, resources(resources)
	, pipeline(pipeline)
	, camera(camera)
{


	lightSpaceViewProj = new ConstantBuffer(dc, resources, pipeline, &lightCam, sizeof(Data), "LightCam");

	CalculateSize();
	CreateRTTs(resizeNotice);
	CreateDepthStateAndView();
	DescViewport(); 
	CreateShader();
}

Shadow::~Shadow()
{
	SAFE_DELETE(lightSpaceViewProj);
	SAFE_DELETE(depthFromLight);
	SAFE_DELETE(shadowShader);
}

void Shadow::CalculateSize()
{
	TL_Math::Vector3 frustumMiddle = camera->data.camPos + TL_Math::Vector3(XMVector3Transform(TL_Math::Vector3(0,0,1), camera->data.viewInv) * (camera->frustumFar + camera->frustumNear) * 0.5f);//frustum middle spot

	TL_Math::Vector3 middlePoint = { 0,0,0 };

	//빛의 역방향을 쳐다 보고 있는 좌표계를 구함
	dir.Normalize();

	TL_Math::Vector3 axisZ = dir;
	TL_Math::Vector3 axisX = TL_Math::Vector3(0,1,0).Cross(axisZ);
	TL_Math::Vector3 axisY = axisZ.Cross(axisX);



	{//빛 좌표계 x축으로 투영
		float max = FLT_MIN;
		float min = FLT_MAX;

		float axisScalar = 0.0f;

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

	{//빛 좌표계 y축으로 투영
		float max = FLT_MIN;
		float min = FLT_MAX;

		float axisScalar = 0.0f;

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

	{//빛 좌표계 z축으로 투영
		float max = FLT_MIN;
		float min = FLT_MAX;

		float axisScalar = 0.0f;

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

		//frustum의 외곽으로 가게끔
		TL_Math::Vector3 temp = XMVectorScale(axisZ, max - max_depth);

		middlePoint += temp;
	}

	lightTransform = TL_Math::Matrix(axisX, axisY, axisZ);
	lightTransform.m[3][0] = middlePoint.x;
	lightTransform.m[3][1] = middlePoint.y;
	lightTransform.m[3][2] = middlePoint.z;

	
	lightCam.viewInv = lightTransform;
	lightCam.view = XMMatrixInverse(nullptr, lightCam.viewInv);
	lightCam.proj = XMMatrixOrthographicLH(width, height, 1.0f, max_depth);
	lightCam.projInv = XMMatrixInverse(nullptr, lightCam.proj);
	lightCam.camPos = middlePoint;

	lightSpaceViewProj->Update(&lightCam, sizeof(Data));

}

void Shadow::ClearRTTs()
{
	depthFromLight->Clear();

	dc->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Shadow::CreateRTTs(OnResizeNotice* resizeNotice)
{
	depthFromLight = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "depthFromLight");
}

void Shadow::Execute()
{
	ClearRTTs();

	CalculateSize();

	depthFromLight->SetRTOnce(0);
	lightSpaceViewProj->SetOnce(TL_Graphics::E_SHADER_TYPE::VS, 0);
	pipeline->SetDepthStencilViewOnce(depthStencilView);


	pipeline->Draw();
}

void Shadow::CreateDepthStateAndView()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;//todo : 여기
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	resources->texture2Ds->Create(depthStencilBuffer, depthBufferDesc);

	resources->depthStencilViews->CreateDefault(depthStencilView, depthStencilBuffer);


}

void Shadow::DescViewport()
{
	viewPort.Width = (float)width;
	viewPort.Height = (float)height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
}

void Shadow::CreateShader()
{
	//shadowShader = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"", "LightDepth");
}
