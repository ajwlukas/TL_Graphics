#pragma once

#include <string>
#include <d3d11.h>

#include "Resources.h"
#include "Export.h"
#include "IMaterial.h"

#include "Texture.h"

#include "ShaderResourceSlotPS.h"

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
	Material(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline
		,const TL_Graphics::MaterialDesc& desc = TL_Graphics::MaterialDesc());
	virtual ~Material();

	virtual void Set(UINT _baseColor_opcityMapSlot = baseColor_opacityMapSlot, UINT _roughness_specular_metallic_AOMapSlot = roughness_specular_metallic_AOMapSlot, UINT _normalMapSlot = normalMapSlot, UINT _emssiveMapSlot = emissiveMapSlot) override;

	virtual void SetBaseColor_Opacity(class TL_Graphics::IShaderResource*) override;
	virtual void SetRoughness_Specular_Metallic_AO(class TL_Graphics::IShaderResource*) override;
	virtual void SetNormal(class TL_Graphics::IShaderResource*) override;
	virtual void SetEmissive(class TL_Graphics::IShaderResource*) override;
	//void SetShader(std::wstring fileName);

	std::wstring pixelShaderName;
private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	//Resource<	ID3D11PixelShader> pixelShader;


	Texture* baseColor_opcityMap= nullptr;
	Texture* roughness_specular_metallic_AOMap = nullptr;
	Texture* normalMap = nullptr;
	Texture* emissiveMap = nullptr;


	Resource<ID3D11Buffer> buffer;
};
