#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "IShaderResource.h"

class Pipeline;

class ShaderResource : public TL_Graphics::IShaderResource
{
public:
	ShaderResource(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize);
	ShaderResource(std::wstring filename);
	virtual ~ShaderResource();

	virtual void Set() override;

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11ShaderResourceView> srv;

	TL_Graphics::E_SHADER_TYPE type;
	UINT slot;
};