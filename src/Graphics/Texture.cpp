#include "pch_dx_11.h"
#include "Texture.h"

#include "Pipeline.h"

Texture::Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::wstring fileName)
	:ShaderResource(dc, resources, pipeline)
	, fileName(fileName)
{
	resources->srvs->GetFromFile(srv, fileName);


	ID3D11Resource* pResource;
	srv.resource->GetResource(&pResource);

	ID3D11Texture2D* pTexture;
	//QueryInterface 함수 잘모름, ChatGPT에서 가져옴
	pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture);

	D3D11_TEXTURE2D_DESC desc;

	pTexture->GetDesc(&desc);

	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pResource);

	sizeX = desc.Width;
	sizeY = desc.Height;
}

Texture::Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:ShaderResource(dc, resources, pipeline)
	, fileName{}
{
}

Texture::~Texture()
{
}

void Texture::Set(TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
	pipeline->SetTexture(this, type, slot);
}
