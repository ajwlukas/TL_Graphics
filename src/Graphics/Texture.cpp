#include "pch_dx_11.h"
#include "Texture.h"

#include "Pipeline.h"

Texture::Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::wstring fileName)
	:ShaderResource(dc, resources, pipeline)
	, fileName(fileName)
{
	resources->srvs->GetFromFile(srv, fileName);

	LoadTexInfo();
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

void Texture::LoadTexInfo()
{

	ID3D11Resource* pResource;
	srv.resource->GetResource(&pResource);

	ID3D11Texture2D* pTexture;
	//QueryInterface �Լ� �߸�, ChatGPT���� ������
	pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture);

	if (pTexture)
	{
		D3D11_TEXTURE2D_DESC desc;
		pTexture->GetDesc(&desc);

		sizeX = desc.Width;
		sizeY = desc.Height;

	}

	ID3D11Texture3D* pTexture3d;
	//QueryInterface �Լ� �߸�, ChatGPT���� ������
	pResource->QueryInterface(__uuidof(ID3D11Texture3D), (void**)&pTexture3d);


	if (pTexture3d)
	{
		D3D11_TEXTURE3D_DESC desc;
		pTexture3d->GetDesc(&desc);

		sizeX = desc.Width;
		sizeY = desc.Height;
		sizeY = desc.Depth;
	}



	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pTexture3d);
	SAFE_RELEASE(pResource);


}
