#include "pch_dx_11.h"
#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize)
	:ShaderResource(dc, resources, pipeline, slot, type)
	,dc(dc)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;

	resources->buffers->Create(buffer, desc, &initData);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = dataSize / 16;//1¿¤·¹¸àÆ®´ç Å©±â = sizeof(float4) = 16

	resources->srvs->Create(srv, srvDesc, buffer);
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::Update(void* data, size_t dataSize)
{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	
			dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
			// Copy Resource Data..
			memcpy(mappedResource.pData, data, dataSize);
	
			// GPU Access UnLock Buffer Data..
			dc->Unmap(buffer, 0);
}
