#include "pch_dx_11.h"
#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(ID3D11Device* device, ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, void* data, size_t dataSize, std::string debugName)
	:ShaderResource(dc, resources, pipeline)
	, dc(dc)
{
	//UINT num = 16384 * 8192;
	//UINT num = 16384 * 16383 + 8193;
	//UINT num = 16384 * 16383 + 12288;

	//D3D11_BUFFER_DESC desc = {};
	//desc.Usage = D3D11_USAGE_DYNAMIC;
	////desc.ByteWidth = dataSize;
	//desc.ByteWidth = 16 * num;
	//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//desc.MiscFlags = 0;
	//desc.StructureByteStride = 0;

	//D3D11_SUBRESOURCE_DATA initData;
	//initData.pSysMem = data;

	//if (data)
	//	resources->buffers->Create(buffer, desc, &initData);
	//else
	//	resources->buffers->Create(buffer, desc, nullptr);

	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//srvDesc.Buffer.FirstElement = 0;
	//srvDesc.Buffer.NumElements = dataSize / 16;//1¿¤·¹¸àÆ®´ç Å©±â = sizeof(float4) = 16
	//srvDesc.Buffer.NumElements = num;//1¿¤·¹¸àÆ®´ç Å©±â = sizeof(float4) = 16
	//


	//device->CreateShaderResourceView(buffer, &srvDesc, srv);
	////resources->srvs->Create(srv, srvDesc, buffer);

	//if (debugName.length() > 0)
	//	srv.resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.length(), debugName.c_str());


	//https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
	 //Create a texture description
	D3D11_TEXTURE1D_DESC textureDesc = {};
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = 2048 ; // Width of the texture
	//textureDesc.Height = 2048; // Width of the texture
	//textureDesc.Depth = 1;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1; // Number of mip levels
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // Texture format
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//textureDesc.SampleDesc.Count = 1;


	HRESULT hr = S_OK;
	if (data)
	{
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = data;
		initData.SysMemPitch = 16384 * sizeof(float);
		 hr = device->CreateTexture1D(&textureDesc, &initData, &texture);
	}
	else
		hr = device->CreateTexture1D(&textureDesc, nullptr, &texture);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvDesc.Texture1DArray.ArraySize = 1;
	srvDesc.Texture1DArray.FirstArraySlice = 0;
	srvDesc.Texture1DArray.MipLevels = 1;
	srvDesc.Texture1DArray.MostDetailedMip = 0;


	device->CreateShaderResourceView(texture, &srvDesc, srv);
	//resources->srvs->Create(srv, srvDesc, buffer);

	if (debugName.length() > 0)
		srv.resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.length(), debugName.c_str());
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::StartPartialUpdate()
{
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	dc->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
}

void TextureBuffer::PartialUpdate(size_t offset, void* data, size_t dataSize)
{
	// Copy Resource Data..
	memcpy((void*)((char*)mappedResource.pData + offset), data, dataSize);
}

void TextureBuffer::EndPartialUpdate()
{
	// GPU Access UnLock Buffer Data..
	dc->Unmap(texture, 0);
}

void TextureBuffer::Set(TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
	ShaderResource::Set(type, slot);
}

void TextureBuffer::Update(void* data, size_t dataSize)
{
	StartPartialUpdate();

	PartialUpdate(0, data, dataSize);

	EndPartialUpdate();
}

