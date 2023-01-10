#pragma once

#include "ShaderResource.h"

class Texture : public ShaderResource
{
public:
	Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::wstring fileName);
	Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~Texture();
	
	virtual void Set(TL_Graphics::E_SHADER_TYPE type,
		UINT slot) override;

	UINT GetSizeX() { return sizeX; }
	UINT GetSizeY() { return sizeY; }

protected:
	std::wstring fileName;

	UINT sizeX = 0, sizeY = 0;
};