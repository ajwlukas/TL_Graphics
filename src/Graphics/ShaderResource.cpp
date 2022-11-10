#include "pch_dx_11.h"
#include "ShaderResource.h"

#include "Pipeline.h"

ShaderResource::ShaderResource(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type)
	:dc(dc)
	,resources(resources)
	,pipeline(pipeline)
	,slot(slot)
	,type(type)
{
}

ShaderResource::~ShaderResource()
{
}

void ShaderResource::Set()
{
	pipeline->SetShaderResource(this);
}