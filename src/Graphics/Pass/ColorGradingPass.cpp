#include "pch_dx_11.h"
#include "ColorGradingPass.h"

#include "Pipeline.h"

ColorGradingPass::ColorGradingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:IRenderPass(dc, resources, pipeline,1,1)
{
	CreateRenderTarget();
	CreateShader();
}

ColorGradingPass::~ColorGradingPass()
{
	SAFE_DELETE(rtts[0]);
	SAFE_DELETE(shaderPS);
}

void ColorGradingPass::Set()
{
	LUT->Set(TL_Graphics::E_SHADER_TYPE::PS, LUTSlot);

	rtts[0]->SetRT(0);

	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);

	shaderPS->Set();
}

void ColorGradingPass::Execute()
{
	ClearRenderTargets();

	Set();
	pipeline->Draw();

	pipeline->UnSetAllRenderTargets();
	pipeline->BindRenderTargets();
}


void ColorGradingPass::CreateRenderTarget()
{
	rtts[0] = new RenderTargetTexture(dc, resources, pipeline, 1.0f, 1.0f, "ColorGrading", DXGI_FORMAT_R8G8B8A8_UNORM);
}

void ColorGradingPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_ColorGrading.hlsl", "ColorGradingPS");
}
