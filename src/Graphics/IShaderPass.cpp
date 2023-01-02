#include "pch_dx_11.h"
#include "IShaderPass.h"

IShaderPass::IShaderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	: dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{
}

IShaderPass::~IShaderPass()
{
}