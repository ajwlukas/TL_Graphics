#pragma once

#include "ShaderResource.h"

class Texture : public ShaderResource
{
public:
	Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName);
	~Texture();
	
private:
	std::wstring fileName;
};