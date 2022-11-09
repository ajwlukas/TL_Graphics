#include "pch_dx_11.h"
#include "ShaderResource.h"

#include "Pipeline.h"

ShaderResource::ShaderResource(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize)
{
	
}

ShaderResource::ShaderResource(std::wstring filename)
{
	resources->srvs->GetFromFile(srv, filename);
}

ShaderResource::~ShaderResource()
{
}

void ShaderResource::Set()
{
	pipeline->SetShaderResource(this);
}