#include "pch_dx_11.h"
#include "BloomPass.h"

BloomPass::BloomPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();
}

BloomPass::~BloomPass()
{
}

void BloomPass::Set()
{
}

void BloomPass::Execute()
{
	//���� �κ��� �����ϰ�(LightPass) LightRT

	//DownSampling�� �ǽ��Ѵ�. �� 3�ܰ��

	//���� ���� �ػ��� RT�� Blur�Ѵ�.

	//�� ���� �ܰ��� RT�� Accumulate�Ѵ�.

	//�� �ؽ��ĸ� Blur�Ѵ�.

	//�� ���� �ܰ��� RT�� Accumulate�Ѵ�.

	//�� �ؽ��ĸ� Blur�Ѵ�.

	//�ؽ��ĸ� upSampling�ϰ� Blur�Ѵ�.

}

void BloomPass::ClearRenderTargets()
{
}

void BloomPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
}

void BloomPass::CreateShader()
{
}
