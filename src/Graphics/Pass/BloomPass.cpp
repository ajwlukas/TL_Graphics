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
	//밝은 부분을 추출하고(LightPass) LightRT

	//DownSampling을 실시한다. 약 3단계로

	//가장 낮은 해상도의 RT를 Blur한다.

	//그 위의 단계의 RT와 Accumulate한다.

	//그 텍스쳐를 Blur한다.

	//그 위의 단계의 RT와 Accumulate한다.

	//그 텍스쳐를 Blur한다.

	//텍스쳐를 upSampling하고 Blur한다.

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
