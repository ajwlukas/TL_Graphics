#include "pch_dx_11.h"
#include "Shader.h"

#include "Pipeline.h"

Shader::Shader(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName, std::string debugName)
	:type(type)
	, dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{

	if (type == TL_Graphics::E_SHADER_TYPE::VS)
		resources->vertexShaders->Get(shader.vs, fileName);
	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
		resources->pixelShaders->Get(shader.ps, fileName);



	if (debugName.length() > 0)
		if (type == TL_Graphics::E_SHADER_TYPE::VS)
			shader.vs.resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.length(), debugName.c_str());
		else if (type == TL_Graphics::E_SHADER_TYPE::PS)
			shader.ps.resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.length(), debugName.c_str());
}

Shader::~Shader()
{
}

void Shader::Set()
{
	pipeline->SetShader(this);
}
