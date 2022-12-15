#pragma once

#include "ShaderResource.h"

class TextureBuffer : public ShaderResource
{
public:
	TextureBuffer(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, void* data, size_t dataSize);
	~TextureBuffer();

	void Map();

	void PartialUpdate(size_t offset, void* data, size_t dataSize);

	virtual void Update(void* data, size_t dataSize) override;

	void UnMap();

private:
	Resource<ID3D11Buffer> buffer;
	ID3D11DeviceContext* dc;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
};