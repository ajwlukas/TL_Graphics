#include "pch_dx_11.h"
#include "ColorGradingPass.h"

#include "Pipeline.h"

ColorGradingPass::ColorGradingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();

	//LUT = new Texture(dc, resources, pipeline, L"Texture/RGBTable16x1.png");
	/*LUT = new Texture(dc, resources, pipeline, L"Texture/LUT_Sepia.png");
	lut_info.width = 16;
	lut_info.height = 1;*/


	/*LUT = new Texture(dc, resources, pipeline, L"Texture/black_LUT.jpg");
	lut_info.width = 8;
	lut_info.height = 8;*/
	
	//LUT = new Texture(dc, resources, pipeline, L"Texture/rainbow.jpg");
	//LUT = new Texture(dc, resources, pipeline, L"Texture/night.jpg");
	//lut_info.width = 16;
	//lut_info.height = 16;

	LUT = new Texture(dc, resources, pipeline, L"Texture/volumeTexture_Rainbow.dds");
	LUT = new Texture(dc, resources, pipeline, L"Texture/volumeTexture_Greenish.dds");
	//LUT = new Texture(dc, resources, pipeline, L"Texture/volumeTexture_Sepia.dds");


	//LUT_Info_buffer = new ConstantBuffer(dc, resources, pipeline, &lut_info, sizeof(LUT_Info));
}

ColorGradingPass::~ColorGradingPass()
{
	//SAFE_DELETE(LUT_Info_buffer);

	SAFE_DELETE(LUT);

	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void ColorGradingPass::Set()
{
	LUT->Set(TL_Graphics::E_SHADER_TYPE::PS, LUTSlot);

	//LUT_Info_buffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);

	rtt->SetRT(0);
	shaderPS->Set();
}

void ColorGradingPass::Execute()
{
	ClearRenderTargets();

	Set();
	pipeline->Draw();
	pipeline->UnSetRenderTarget(0);
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);

}

void ColorGradingPass::ClearRenderTargets()
{
	rtt->Clear();
}

void ColorGradingPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice);
}

void ColorGradingPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_ColorGrading.hlsl");
}
