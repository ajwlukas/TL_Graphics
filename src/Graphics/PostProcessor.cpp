#include "pch_dx_11.h"
#include "PostProcessor.h"

#include "Pipeline.h"

PostProcessor::PostProcessor(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{
	screenMesh = new ScreenMesh(dc, resources, pipeline);

	deferredRenderPass = new DeferredRenderPass(dc, resources, pipeline, resizeNotice);

	gridPass = new GridPass(dc, resources, pipeline, resizeNotice);

	downSamplerPass = new SamplerPass(dc, resources, pipeline, resizeNotice, 0.5f, 0.5f);
	//downSamplerPass = new SamplerPass(dc, resources, pipeline, resizeNotice, (UINT)1, (UINT)1);

	colorGradingPass = new ColorGradingPass(dc, resources, pipeline, resizeNotice);

	gaussianBlurPassX = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 1,0 });

	gaussianBlurPassY = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 0,1 });

	lightAdaptionPass = new LightAdaptionPass(dc, resources, pipeline, resizeNotice);

	finalPass = new FinalPass(dc, resources, pipeline, resizeNotice);

	cubeMapPass = new CubeMapPass(dc, resources, pipeline, resizeNotice, L"_DevelopmentAssets/Texture/CubeMaps/GardenEnvHDR.dds");
}

PostProcessor::~PostProcessor()
{
	SAFE_DELETE(finalPass);

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

void PostProcessor::Execute()
{
	pipeline->SetDepthDisabled();

	if (cubeMapPass)
		cubeMapPass->Execute();

	screenMesh->Set();


	if (control.doGrid)
		gridPass->Execute();

	deferredRenderPass->Execute();

	if (control.doDownSample)
	downSamplerPass->Execute();

	if (control.doColorGrading)
	colorGradingPass->Execute();

	if (control.doGaussianBlur)
	{
		gaussianBlurPassX->Execute();
		gaussianBlurPassY->Execute();
	}
	/*if (control.doDownSample)

	lightAdaptionPass->Execute();*/

	finalPass->Execute();
	UnBindGBuffers();
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
