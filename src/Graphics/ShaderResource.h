#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "IShaderResource.h"

class Pipeline;

class ShaderResource : public TL_Graphics::IShaderResource
{
public:
	ShaderResource(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type);
	virtual ~ShaderResource();

	virtual void Set() override;
	virtual void Update(void* data, size_t dataSize) override {};

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;


	TL_Graphics::E_SHADER_TYPE type;
	UINT slot;

protected:
	Resource<ID3D11ShaderResourceView> srv;
};