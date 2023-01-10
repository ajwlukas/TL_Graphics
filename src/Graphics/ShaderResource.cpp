#include "pch_dx_11.h"
#include "ShaderResource.h"

#include "Pipeline.h"

ShaderResource::ShaderResource(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:dc(dc)
	,resources(resources)
	,pipeline(pipeline)
{
}

ShaderResource::~ShaderResource()
{
}

void ShaderResource::Set(TL_Graphics::E_SHADER_TYPE type,
	UINT slot)
{

	//D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	//srv.resource->GetDesc(&desc);

	pipeline->SetShaderResource(this, type, slot);
}