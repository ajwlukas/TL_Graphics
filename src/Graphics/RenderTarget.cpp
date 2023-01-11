#include "pch_dx_11.h"
#include "RenderTarget.h"

#include "Pipeline.h"


RenderTarget::RenderTarget(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Set(UINT slot)
{
	pipeline->SetRenderTarget(this, slot);
}

void RenderTarget::Clear(TL_Math::Vector4 color)
{
	pipeline->ClearRenderTarget(this, color);
}
