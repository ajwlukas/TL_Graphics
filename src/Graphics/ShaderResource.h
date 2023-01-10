#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "IShaderResource.h"

class Pipeline;

//todo : 인터페이스로 빼주지 말자

class ShaderResource : public TL_Graphics::IShaderResource
{
public:
	ShaderResource(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	virtual ~ShaderResource();

	virtual void Set(TL_Graphics::E_SHADER_TYPE type,
		UINT slot) override;
	virtual void Update(void* data, size_t dataSize) override {};

protected:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11ShaderResourceView> srv;
};