#include "pch_dx_11.h"
#include "Material.h"

#include "Pipeline.h"

Material::Material(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline
	, const TL_Graphics::MaterialDesc& desc)
	: dc(dc)
	, pixelShaderName(pixelShaderName)
	, resources(resources)
	, pipeline(pipeline)
{
	 //resources->pixelShaders->Get(pixelShader, pixelShaderName);

	 if(desc.albedoMapFileName.length() > 0)
	 albedoMap = new Texture(dc, resources, pipeline, desc.albedoMapFileName);

	 if(desc.metallicMapFileName.length() > 0)
		 metallicMap = new Texture(dc, resources, pipeline, desc.metallicMapFileName);

	 if(desc.roughnessMapFileName.length() > 0)
		 roughnessMap = new Texture(dc, resources, pipeline, desc.roughnessMapFileName);
}

Material::~Material()
{
	SAFE_DELETE(albedoMap);
	SAFE_DELETE(metallicMap);
	SAFE_DELETE(roughnessMap);
}

void Material::Set(UINT albdeoMapSlot, UINT metallicMapSlot, UINT roughnessMapSlot)
{
	pipeline->SetMaterial(this);

	if(albedoMap)
	albedoMap->Set(TL_Graphics::E_SHADER_TYPE::PS, albdeoMapSlot);

	if(metallicMap)
	metallicMap->Set(TL_Graphics::E_SHADER_TYPE::PS, metallicMapSlot);

	if(roughnessMap)
	roughnessMap->Set(TL_Graphics::E_SHADER_TYPE::PS, roughnessMapSlot);
}

void Material::SetAlbedo(class TL_Graphics::IShaderResource* albedo)
{
	albedoMap = reinterpret_cast<Texture*>(albedo);
}

void Material::SetMetallic(class TL_Graphics::IShaderResource* metallic)
{
	metallicMap = reinterpret_cast<Texture*>(metallic);
}

void Material::SetRoughness(class TL_Graphics::IShaderResource* roughness)
{
	roughnessMap = reinterpret_cast<Texture*>(roughness);
}

//void Material::SetShader(wstring fileName)
//{
//	pixelShaderName = fileName;
//	resources->pixelShaders->Get(pixelShader, fileName);
//}
