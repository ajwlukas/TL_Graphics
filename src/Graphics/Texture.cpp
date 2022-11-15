#include "pch_dx_11.h"
#include "Texture.h"


Texture::Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::wstring fileName)
	:ShaderResource(dc, resources, pipeline)
	, fileName(fileName)
{
	resources->srvs->GetFromFile(srv, fileName);
}

Texture::~Texture()
{
}