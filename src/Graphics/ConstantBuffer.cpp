#include "pch_dx_11.h"
#include "ConstantBuffer.h"

#include "Pipeline.h"

ConstantBuffer::ConstantBuffer(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, void* data, size_t dataSize, std::string debugName)
	:dc(dc)
	,resources(resources)
	,pipeline(pipeline)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = data;

	resources->buffers->Create(buffer, desc, &initData);

	if(debugName.length() >0)
	buffer.resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.length(), debugName.c_str());
}

ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::Set(TL_Graphics::E_SHADER_TYPE type,
	UINT slot)
{
	pipeline->SetConstantBuffer(this, type, slot);
}

void ConstantBuffer::Update(void* data, size_t dataSize)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Copy Resource Data..
	memcpy(mappedResource.pData, data, dataSize);

	// GPU Access UnLock Buffer Data..
	dc->Unmap(buffer, 0);
}
