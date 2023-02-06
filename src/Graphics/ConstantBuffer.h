#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "IConstantBuffer.h"

class Pipeline;

class ConstantBuffer : public TL_Graphics::IConstantBuffer
{
public:
	ConstantBuffer(ID3D11DeviceContext* dc,Resources* resources, Pipeline* pipeline, void* data, size_t dataSize, std::string debugName = "");
	virtual ~ConstantBuffer();

	virtual void Set(TL_Graphics::E_SHADER_TYPE type,
		UINT slot) override;
	virtual void Update(void* data, size_t dataSize) override;
	

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11Buffer> buffer;
};