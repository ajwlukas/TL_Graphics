#include "pch_dx_11.h"
#include "IRenderPass.h"

IRenderPass::IRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	: dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{
}

IRenderPass::~IRenderPass()
{
}