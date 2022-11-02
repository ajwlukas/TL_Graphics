#include "pch_dx_11.h"
#include "Material.h"

Material::Material(ID3D11DeviceContext* deviceContext, Resources* resources, Pipeline* pipeline, std::wstring pixelShaderName,
	D3D11_SAMPLER_DESC samplerDesc
	, const TL_Graphics::MaterialDesc& desc)
	:normal{}, diffuse{}, specular{},
	samplerState{}, pixelShader(nullptr)
	, pixelShaderName(pixelShaderName)
	, dc(deviceContext)
	, resources(resources)
	, pipeline(pipeline)
{
	pixelShader = resources->pixelShaders->Get(pixelShaderName);

	if (desc.diffuseFileName.length() > 0)
	{ 
		resources->srvs->GetFromFile(diffuse, desc.diffuseFileName);//todo : ������� �� ������ �ȳ�
	}

	if (desc.normalFileName.length() > 0)
	{ 
		resources->srvs->GetFromFile(normal, desc.normalFileName);//todo : ������� �� ������ �ȳ�
	}

	resources->samplerStates->Get(samplerState, samplerDesc);

	memcpy(data.ambient, desc.ambient, sizeof(desc.ambient))		  ;
	memcpy(data.diffuse, desc.diffuse, sizeof(desc.diffuse))			;
	memcpy(data.specular, desc.specular, sizeof(desc.specular));
		/*data.ambient = desc.ambient ;
		data.diffuse = desc.diffuse	 ;
		data.specular = desc.specular;*/

		D3D11_BUFFER_DESC cbd;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.ByteWidth = sizeof(Data);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = 0;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &data;
		resources->buffers->Create(buffer, cbd, &initData);
}

Material::~Material()
{
}

void Material::Set()
{//todo
	if (diffuse.resource != nullptr)
		dc->PSSetShaderResources(0, 1, diffuse);
	if (normal.resource != nullptr)
		dc->PSSetShaderResources(1, 1, normal);
	if (specular.resource != nullptr)
		dc->PSSetShaderResources(2, 1, specular);

		dc->PSSetSamplers(0, 1, samplerState);
		dc->PSSetShader(pixelShader,0,0);

		dc->PSSetConstantBuffers(0, 1, buffer);

		pipeline->SetMaterial(this);
}

void Material::SetShader(wstring fileName)
{
	pixelShaderName = fileName;
	pixelShader = resources->pixelShaders->Get(fileName);
}

void Material::SetDiffuseMap(wstring fileName)
{
	//return;//todo : �ӽ� �ڵ�
	resources->srvs->GetFromFile(diffuse, fileName);
}

void Material::SetNormalMap(wstring fileName)
{
	resources->srvs->GetFromFile(normal, fileName);
}

void Material::SetSpecularMap(wstring fileName)
{
	resources->srvs->GetFromFile(specular, fileName);
}

void Material::SetSamplerState(D3D11_SAMPLER_DESC samplerDesc)
{
	resources->samplerStates->Get(samplerState, samplerDesc);
}
