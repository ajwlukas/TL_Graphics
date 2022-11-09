#pragma once

#include <string>
#include <d3d11.h>

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
	Material( Resources* resources, Pipeline* pipeline, std::wstring pixelShaderName
		,const TL_Graphics::MaterialDesc& desc = TL_Graphics::MaterialDesc());
	virtual ~Material();

	virtual void Set() override;

	void SetShader(std::wstring fileName);

	void SetDiffuseMap(std::wstring fileName);
	void SetNormalMap(std::wstring fileName);
	void SetSpecularMap(std::wstring fileName);

	struct Data
	{
		TL_Math::Vector4 ambient;
		TL_Math::Vector4 diffuse;
		TL_Math::Vector4 specular;
	}data;

	std::wstring pixelShaderName;
private:
	friend class Pipeline;
	Resources* resources;
	Pipeline* pipeline;

	ID3D11PixelShader* pixelShader;
	Resource<ID3D11ShaderResourceView> diffuse;
	Resource<ID3D11ShaderResourceView> normal;
	Resource<ID3D11ShaderResourceView> specular;


	Resource<ID3D11Buffer> buffer;
};
