#pragma once

#include <string>
#include <d3d11.h>

#include "Resources.h"
#include "Export.h"
#include "IMaterial.h"

#include "Texture.h"

/// <summary>
/// Material 생성하는 2가지 방법,
/// 1. MaterialDesc에 데이터를 작성해서 Material 생성자에 넣어줘서 생성
/// 2. Material을 생성후 Set함수들로 데이터 입력
/// 
/// written by Ssreppa 22.06.22
/// </summary>

class Pipeline;

class Material : public TL_Graphics::IMaterial
{
public:
	Material(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::wstring pixelShaderName
		,const TL_Graphics::MaterialDesc& desc = TL_Graphics::MaterialDesc());
	virtual ~Material();

	virtual void Set(UINT albdeoMapSlot = 0, UINT metallicMapSlot = 1, UINT roughnessMapSlot = 2) override;

	//void SetShader(std::wstring fileName);

	std::wstring pixelShaderName;
private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<	ID3D11PixelShader> pixelShader;


	Texture* albedoMap	 = nullptr;
	Texture* metallicMap = nullptr;
	Texture* roughnessMap = nullptr;


	Resource<ID3D11Buffer> buffer;
};
