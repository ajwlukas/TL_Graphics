#include "pch_dx_11.h"
#include "PostProcessor.h"

#include "Pipeline.h"

PostProcessor::PostProcessor(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{

	gBufferRenderPass = new GBufferRenderPass(dc, resources, pipeline);

	screenMesh = new ScreenMesh(dc, resources, pipeline);

	deferredRenderPass = new DeferredRenderPass(dc, resources, pipeline);

	gridPass = new GridPass(dc, resources, pipeline);

	downSamplerPass = new SamplerPass(dc, resources, pipeline);
	//downSamplerPass->SetRatio(0.5f, 0.5f);

	colorGradingPass = new ColorGradingPass(dc, resources, pipeline);

	gaussianBlurPassX = new GaussianBlurPass(dc, resources, pipeline, { 1,0 });

	gaussianBlurPassY = new GaussianBlurPass(dc, resources, pipeline, { 0,1 });

	lightAdaptionPass = new LightAdaptionPass(dc, resources, pipeline);

	finalPass = new FinalPass(dc, resources, pipeline);

	cubeMapPass = new CubeMapPass(dc, resources, pipeline);

	bloomPass = new BloomPass(dc, resources, pipeline);
	bloomPass->CreateDestTexture(0, "BloomRT");

	lightPass = new LightPass(dc, resources, pipeline);

	greyScalePass = new GreyScalePass(dc, resources, pipeline);
	greyScalePass->CreateDestTexture(0, "GreyScale", DXGI_FORMAT_R32_FLOAT);

	averagePass = new AveragePass(dc, resources, pipeline);
	averagePass->CreateDestTexture(0, "Average");

	luminancePass = new LuminancePass(dc, resources, pipeline);
	luminancePass->CreateDestTexture(0, "Luminance");
	luminancePass->Init();

	lightAdaptionPass = new LightAdaptionPass(dc, resources, pipeline);
	lightAdaptionPass->CreateDestTexture(0, "LightAdaption");

	toneMappingPass = new ToneMappingPass(dc, resources, pipeline);
	toneMappingPass->CreateDestTexture(0, "toneMapping");
}

PostProcessor::~PostProcessor()
{
	bloomPass->DeleteDestTextures();

	SAFE_DELETE(toneMappingPass);

	SAFE_DELETE(lightAdaptionPass);

	SAFE_DELETE(averagePass);

	SAFE_DELETE(luminancePass);

	SAFE_DELETE(greyScalePass);

	SAFE_DELETE(gBufferRenderPass);

	SAFE_DELETE(finalPass);

	SAFE_DELETE(lightPass);

	SAFE_DELETE(bloomPass);

	SAFE_DELETE(lightAdaptionPass);

	SAFE_DELETE(cubeMapPass);

	SAFE_DELETE(gaussianBlurPassX);
	SAFE_DELETE(gaussianBlurPassY);
	SAFE_DELETE(colorGradingPass);
	SAFE_DELETE(downSamplerPass);
	SAFE_DELETE(gridPass);
	SAFE_DELETE(deferredRenderPass);
	SAFE_DELETE(screenMesh);
}

void PostProcessor::CollectGBufferInfos()
{
	gBufferRenderPass->ClearRenderTargets();
	gBufferRenderPass->Set();//화면 기하정보 뽑아냄
}

void PostProcessor::Execute()
{
	pipeline->SetDepthDisabled();
	auto oldDSView = pipeline->SetDepthStencilView(nullptr);

	cubeMapPass->SetSourceTexture(control.cubeMap, 0);
	cubeMapPass->SetSourceTexture(control.irradianceMap, 1);
	cubeMapPass->SetSourceTexture(control.prefilteredEnvMap, 2);
	cubeMapPass->SetSourceTexture(control.iblBRDF, 3);
	cubeMapPass->Execute();
	RenderTargetTexture* cubeMap = cubeMapPass->GetDestTexture();

	screenMesh->Set();

	for (UINT i = 0; i < 8; i++)
		deferredRenderPass->SetSourceTexture(gBufferRenderPass->GetDestTexture(i), i);
	deferredRenderPass->SetDestTexture(cubeMap);
	deferredRenderPass->Execute();
	RenderTargetTexture* proto = deferredRenderPass->GetDestTexture();

	///여기부터 후처리
	RenderTargetTexture* before = proto;

	if(control.doLightAdaption)
	{
		luminancePass->SetSourceTexture(before);
		luminancePass->Execute();
		Texture* luminance = luminancePass->GetDestTexture();

		lightAdaptionPass->SetSourceTexture(before, 0);
		lightAdaptionPass->SetSourceTexture(luminance, 1);

		lightAdaptionPass->SetMiddleGrey(control.middleGrey);

		lightAdaptionPass->Execute();
		before = lightAdaptionPass->GetDestTexture();
	}

	if (control.doToneMapping)
	{
		toneMappingPass->SetSourceTexture(before, 0);

		toneMappingPass->SetMaxWhite(control.maxWhite);

		toneMappingPass->Execute();
		before = toneMappingPass->GetDestTexture();
	}


	if (control.doBloom)
	{
		bloomPass->SetSourceTexture(before, 0);
		bloomPass->Execute();
		before = bloomPass->GetDestTexture();
	}


	if (control.doColorGrading)
	{
		colorGradingPass->SetSourceTexture(before);
		colorGradingPass->SetLUT(control.colorGradingLUT);

		colorGradingPass->Execute();

		before = colorGradingPass->GetDestTexture();
	}


	finalPass->SetSourceTexture(before);
	finalPass->Execute();


	gBufferRenderPass->ClearRenderTargets();
	//정리
	UnBindGBuffers();
	pipeline->SetDepthStencilView(oldDSView);
}

void PostProcessor::UnBindGBuffers()
{
	for (int i = 0; i < 8; i++)
	{
		pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 20 + i);
	}

	pipeline->SetShaderResource(nullptr,
		TL_Graphics::E_SHADER_TYPE::PS, 31);

	pipeline->BindShaderResourcesPS();
}