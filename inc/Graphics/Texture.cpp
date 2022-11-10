#include "pch_dx_11.h"
#include "Texture.h"


Texture::Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName)
	:ShaderResource(dc, resources, pipeline, slot, type)
	, fileName(fileName)
{
	resources->srvs->GetFromFile(srv, fileName);
}

Texture::~Texture()
{
}