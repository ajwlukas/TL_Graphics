#include "pch_dx_11.h"
#include "AccumulatorPass.h"

AccumulatorPass::AccumulatorPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 2)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();
}

AccumulatorPass::~AccumulatorPass()
{
}

void AccumulatorPass::Set()
{
}

void AccumulatorPass::Execute()
{
	//todo : ¿©±â
}

void AccumulatorPass::ClearRenderTargets()
{
}

void AccumulatorPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "Accumulator");
}

void AccumulatorPass::CreateShader()
{
}
