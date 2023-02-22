#include "pch_dx_11.h"
#include "ToneMappingPass.h"

ToneMappingPass::ToneMappingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
{
}

ToneMappingPass::~ToneMappingPass()
{
}

void ToneMappingPass::Set()
{
}

void ToneMappingPass::Execute()
{
}

void ToneMappingPass::CreateRenderTargets(OnResizeNotice* resizeNotice)
{
}

void ToneMappingPass::DeleteRenderTargets()
{
}

void ToneMappingPass::CreateShader()
{
}
