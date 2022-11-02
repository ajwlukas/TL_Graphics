#pragma once

#include <string>
#include <d3d11.h>

#include "typedef.h"
#include "Resources.h"
#include "Export.h"
#include "IMaterial.h"

/// <summary>
/// Material �����ϴ� 2���� ���,
/// 1. MaterialDesc�� �����͸� �ۼ��ؼ� Material �����ڿ� �־��༭ ����
/// 2. Material�� ������ Set�Լ���� ������ �Է�
/// 
/// written by Ssreppa 22.06.22
/// </summary>

class Pipeline;

class Material : public TL_Graphics::IMaterial
{
public:
	Material(ID3D11DeviceContext* deviceContext, Resources* resources, Pipeline* pipeline, std::wstring pixelShaderName,
		D3D11_SAMPLER_DESC samplerDesc, const TL_Graphics::MaterialDesc& desc = TL_Graphics::MaterialDesc());
	virtual ~Material();

	virtual void Set() override;

	void SetShader(std::wstring fileName);

	void SetDiffuseMap(std::wstring fileName);
	void SetNormalMap(std::wstring fileName);
	void SetSpecularMap(std::wstring fileName);


	void SetSamplerState(D3D11_SAMPLER_DESC samplerDesc);

	struct Data
	{
		float ambient[4];
		float diffuse[4];
		float specular[4];
	}data;

	std::wstring pixelShaderName;
private:
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	ID3D11PixelShader* pixelShader;
	Resource<ID3D11ShaderResourceView> diffuse;
	Resource<ID3D11ShaderResourceView> normal;
	Resource<ID3D11ShaderResourceView> specular;
	Resource<ID3D11SamplerState> samplerState;


	Resource<ID3D11Buffer> buffer;
};
