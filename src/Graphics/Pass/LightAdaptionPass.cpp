#include "pch_dx_11.h"
#include "LightAdaptionPass.h"

#include "Pipeline.h"

LightAdaptionPass::LightAdaptionPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();

	UINT size = 1;


	for (UINT i = 0; i < downSamplingCount - 1; i++)
	{
		downSamplers[i] = new SamplerPass(dc, resources, pipeline, resizeNotice, size, size, downSampler3x3);

		size *= 3;
	}

	downSamplers[downSamplingCount - 1] = new SamplerPass(dc, resources, pipeline, resizeNotice, size, size);
}

LightAdaptionPass::~LightAdaptionPass()
{
	for (UINT i = 0; i < 6; i++)
		SAFE_DELETE(downSamplers[i]);

	SAFE_DELETE(downSampler3x3);
	SAFE_DELETE(downSamplerLuminance);

	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void LightAdaptionPass::Set()
{
	rtt->SetRT(0);
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
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);
}

void LightAdaptionPass::ClearRenderTargets()
{
	rtt->Clear();

	for (UINT i = 0; i < downSamplingCount; i++)
		downSamplers[i]->ClearRenderTargets();
}

void LightAdaptionPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice);
}

void LightAdaptionPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_LightAdaptionPass.hlsl");

	downSampler3x3 = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_DownSampler3x3Average.hlsl");

	downSampler3x3 = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_Luminance.hlsl");
}
