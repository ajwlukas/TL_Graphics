#include "pch_dx_11.h"
#include "LightAdaptionPass.h"

#include "Pipeline.h"

LightAdaptionPass::LightAdaptionPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();

	UINT size = 1;


	for (UINT i = 0; i < downSamplingCount - 1; i++)
	{
		//downSamplers[i] = new SamplerPass(dc, resources, pipeline, resizeNotice, size, size, downSampler3x3);
		//todo : 여기 대충 고침 무조건!!!!!!!!!!!!! 다시 할 것
		downSamplers[i] = new SamplerPass(dc, resources, pipeline, resizeNotice, downSampler3x3);

		size *= 3;
	}

	//downSamplers[downSamplingCount - 1] = new SamplerPass(dc, resources, pipeline, resizeNotice, size, size);
	downSamplers[downSamplingCount - 1] = new SamplerPass(dc, resources, pipeline, resizeNotice);
}

LightAdaptionPass::~LightAdaptionPass()
{
	for (UINT i = 0; i < 6; i++)
		SAFE_DELETE(downSamplers[i]);

	SAFE_DELETE(downSampler3x3);
	SAFE_DELETE(downSamplerLuminance);

	SAFE_DELETE(rtts[0]);
	SAFE_DELETE(shaderPS);
}

void LightAdaptionPass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();
}

void LightAdaptionPass::Execute()
{
	for (int i = downSamplingCount - 1; i >= 0; i--)
		downSamplers[i]->Execute();


	ClearRenderTargets();

	Set();
	pipeline->Draw();
	pipeline->UnSetRenderTarget(0);
	rtts[0]->SetT(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
}

void LightAdaptionPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice);
}

void LightAdaptionPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_LightAdaptionPass.hlsl");

	downSampler3x3 = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_DownSampler3x3Average.hlsl");

	downSampler3x3 = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_Luminance.hlsl");
}
