#include "pch_dx_11.h"
#include "BloomPass.h"

#include "Pipeline.h"

BloomPass::BloomPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:IRenderPass(dc, resources, pipeline, 1,1)
{

	gaussianBlurPassX = new GaussianBlurPass(dc, resources, pipeline, { 1,0 });
	gaussianBlurPassY = new GaussianBlurPass(dc, resources, pipeline, { 0,1 });

	accumulatorPass = new AccumulatorPass(dc, resources, pipeline) ;
	accumulatorPass0 = new AccumulatorPass(dc, resources, pipeline);

	samplerPass = new SamplerPass(dc, resources, pipeline);

	lightPass = new LightPass(dc, resources, pipeline);

	CreateRenderTargets();
	CreateShader();
}

BloomPass::~BloomPass()
{
	DeleteRenderTargets();

	SAFE_DELETE(lightPass);
	SAFE_DELETE(samplerPass);
	//SAFE_DELETE(samplerPass0);
	SAFE_DELETE(accumulatorPass);
	SAFE_DELETE(accumulatorPass0);
	SAFE_DELETE(gaussianBlurPassX);
	SAFE_DELETE(gaussianBlurPassY);
}

void BloomPass::Set()
{
}

void BloomPass::Execute()
{
	ClearRenderTargets();

	//밝은 부분을 추출하고(LightPass) LightRT

	//DownSampling을 실시한다. 약 3단계로
	lightPass->SetSourceTexture(sourceTextures[0]);
	lightPass->Execute();
	Texture* lightmap = lightPass->GetDestTexture();


	for (int i = 1; i < 4; i++)
	{
		//가장 낮은 해상도의 RT를 Blur한다.

		float div = 0.25f * i;
		float div1 = 0.25f * (i + 1);

		gaussianBlurPassX->SetRatio(div, div);
		gaussianBlurPassX->SetSourceTexture(lightmap);
		gaussianBlurPassX->Execute();

		gaussianBlurPassY->SetRatio(div, div);
		gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
		gaussianBlurPassY->Execute();


		//그 위의 단계의 RT와 Accumulate한다.
		samplerPass->SetRatio(div1, div1);
		samplerPass->SetSourceTexture(lightmap);
		samplerPass->Execute();
		accumulatorPass->SetRatio(div1, div1);
		accumulatorPass->SetSourceTexture(gaussianBlurPassY->GetDestTexture(),0);
		accumulatorPass->SetSourceTexture(samplerPass->GetDestTexture(),1);
		accumulatorPass->Execute();

		gaussianBlurPassX->ClearRenderTargets();
		samplerPass->ClearRenderTargets();
		gaussianBlurPassY->ClearRenderTargets();

	}

	//마지막으로 원본과 합쳐준다.
	accumulatorPass0->SetDestTexture(rtts[0]);
	accumulatorPass0->SetSourceTexture(accumulatorPass->GetDestTexture(), 0);
	accumulatorPass0->SetSourceTexture(sourceTextures[0], 1);
	accumulatorPass0->Execute();

	accumulatorPass->ClearRenderTargets();

	pipeline->UnSetAllRenderTargets();
	pipeline->BindRenderTargets();
}

void BloomPass::CreateRenderTargets()
{
	samplerPass->CreateDestTexture(0,"Bloom_Sampler");
	gaussianBlurPassX->CreateDestTexture(0,"Bloom_GaussianX");
	gaussianBlurPassY->CreateDestTexture(0,"Bloom_GaussianY");
	accumulatorPass->CreateDestTexture(0,"Bloom_Accumulator");
	//accumulatorPass0->CreateDestTexture(0,"BloomSampler0");
	lightPass->CreateDestTexture(0,"Bloom_LightPass");
}

void BloomPass::DeleteRenderTargets()
{
	samplerPass->DeleteDestTextures();
	gaussianBlurPassX->DeleteDestTextures();
	gaussianBlurPassY->DeleteDestTextures();
	accumulatorPass->DeleteDestTextures();
	//accumulatorPass0->DeleteDestTextures();
	lightPass->DeleteDestTextures();
}

void BloomPass::CreateShader()
{
}
