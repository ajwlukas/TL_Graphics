#pragma once

#include "ShaderResource.h"

class Texture : public ShaderResource
{
public:
	Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::wstring fileName);
	~Texture();
	
private:
	std::wstring fileName;
};