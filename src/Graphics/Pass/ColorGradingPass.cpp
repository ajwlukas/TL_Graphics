#include "pch_dx_11.h"
#include "ColorGradingPass.h"

#include "Pipeline.h"

ColorGradingPass::ColorGradingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();


	LUT = new Texture(dc, resources, pipeline, L"_DevelopmentAssets/Texture/volumeTexture_Greenish.dds", "ColorGradingLUT");
}

ColorGradingPass::~ColorGradingPass()
{
	SAFE_DELETE(LUT);

	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void ColorGradingPass::Set()
{
	LUT->Set(TL_Graphics::E_SHADER_TYPE::PS, LUTSlot);

	rtt->SetRT(0);
	shaderPS->Set();
}

void ColorGradingPass::Execute()
{
	ClearRenderTargets();

	Set();
	pipeline->Draw();
	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);
}

void ColorGradingPass::ClearRenderTargets()
{
	rtt->Clear();
}

void ColorGradingPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "ColorGrading");
}

void ColorGradingPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_ColorGrading.hlsl", "ColorGradingPS");
}
