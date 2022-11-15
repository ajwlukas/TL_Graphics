#pragma once

#include "ShaderResource.h"

class RenderTargetTexture : public ShaderResource
{
public:
	RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName);
	~RenderTargetTexture();

private:
	Resource<ID3D11Texture2D> texture;
	Resource<ID3D11RenderTargetView> rtv;

};