#include "pch_dx_11.h"
#include "BloomPass.h"

BloomPass::BloomPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice,1,1)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();

	gaussianBlurPassX = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 1,0 });
	gaussianBlurPassY = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 0,1 });

	accumulatorPass = new AccumulatorPass(dc, resources, pipeline, resizeNotice);

	samplerPass = new SamplerPass(dc, resources, pipeline, resizeNotice);
}

BloomPass::~BloomPass()
{
	SAFE_DELETE(samplerPass);
	SAFE_DELETE(accumulatorPass);
	SAFE_DELETE(gaussianBlurPassX);
	SAFE_DELETE(gaussianBlurPassY);
}

void BloomPass::Set()
{
}

void BloomPass::Execute()
{
	//밝은 부분을 추출하고(LightPass) LightRT

	//DownSampling을 실시한다. 약 3단계로

	Texture* lightmap = nullptr;

	//가장 낮은 해상도의 RT를 Blur한다.
	gaussianBlurPassX->SetRatio(0.25f, 0.25f);
	gaussianBlurPassX->SetSourceTexture(lightmap);
	gaussianBlurPassX->Execute();

	gaussianBlurPassY->SetRatio(0.25f, 0.25f);
	gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
	gaussianBlurPassY->Execute();

	//그 위의 단계의 RT와 Accumulate한다.
	samplerPass->SetRatio(0.5f, 0.5f);
	samplerPass->SetSourceTexture(lightmap);
	samplerPass->Execute();
	accumulatorPass->SetRatio(0.5f, 0.5f);
	accumulatorPass->SetSourceTexture(gaussianBlurPassY->GetDestTexture(),0);
	accumulatorPass->SetSourceTexture(samplerPass->GetDestTexture(),1);
	accumulatorPass->Execute();

	//그 텍스쳐를 Blur한다.
	gaussianBlurPassX->SetRatio(0.5f, 0.5f);
	gaussianBlurPassX->SetSourceTexture(accumulatorPass->GetDestTexture());
	gaussianBlurPassX->Execute();

	gaussianBlurPassY->SetRatio(0.5f, 0.5f);
	gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
	gaussianBlurPassY->Execute();

	//그 위의 단계의 RT와 Accumulate한다.
	samplerPass->SetRatio(0.5f, 0.5f);
	samplerPass->SetSourceTexture(lightmap);
	samplerPass->Execute();
	accumulatorPass->SetRatio(0.75f, 0.75f);
	accumulatorPass->SetSourceTexture(gaussianBlurPassY->GetDestTexture(),0);
	accumulatorPass->SetSourceTexture(samplerPass->GetDestTexture(),1);
	accumulatorPass->Execute();

	//그 텍스쳐를 Blur한다.
	gaussianBlurPassX->SetRatio(0.75f, 0.75f);
	gaussianBlurPassX->SetSourceTexture(accumulatorPass->GetDestTexture());
	gaussianBlurPassX->Execute();

	gaussianBlurPassY->SetRatio(0.75f, 0.75f);
	gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
	gaussianBlurPassY->Execute();

	//그 위의 단계의 RT와 Accumulate한다.
	samplerPass->SetRatio(1.0f, 1.0f);
	samplerPass->SetSourceTexture(lightmap);
	samplerPass->Execute();
	accumulatorPass->SetRatio(1.0f, 1.0f);
	accumulatorPass->SetSourceTexture(gaussianBlurPassY->GetDestTexture(), 0);
	accumulatorPass->SetSourceTexture(samplerPass->GetDestTexture(), 1);
	accumulatorPass->Execute();

}

void BloomPass::ClearRenderTargets()
{
	//todo : 이거 IPass로 빼주자
}

void BloomPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
}

void BloomPass::CreateShader()
{
}
