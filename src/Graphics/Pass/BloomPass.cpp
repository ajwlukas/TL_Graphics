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

	//���� �κ��� �����ϰ�(LightPass) LightRT

	//DownSampling�� �ǽ��Ѵ�. �� 3�ܰ��
	lightPass->SetSourceTexture(sourceTextures[0]);
	lightPass->Execute();
	Texture* lightmap = lightPass->GetDestTexture();


	for (int i = 1; i < 4; i++)
	{
		//���� ���� �ػ��� RT�� Blur�Ѵ�.

		float div = 0.25f * i;
		float div1 = 0.25f * (i + 1);

		gaussianBlurPassX->SetRatio(div, div);
		gaussianBlurPassX->SetSourceTexture(lightmap);
		gaussianBlurPassX->Execute();

		gaussianBlurPassY->SetRatio(div, div);
		gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
		gaussianBlurPassY->Execute();


		//�� ���� �ܰ��� RT�� Accumulate�Ѵ�.
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

	//���������� ������ �����ش�.
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
