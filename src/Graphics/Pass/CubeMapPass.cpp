#include "pch_dx_11.h"
#include "CubeMapPass.h"

#include "Pipeline.h"

#include "Vertex.h"

CubeMapPass::CubeMapPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:IRenderPass(dc, resources, pipeline, 1, 4)
{
	cubeMap = new Texture(dc, resources, pipeline, L"_DevelopmentAssets/Texture/CubeMaps/ValleyEnvHDR.dds");
	irradianceMap = new Texture(dc, resources, pipeline, L"_DevelopmentAssets/Texture/CubeMaps/ValleyDiffuseHDR.dds");
	prefilteredEnvMap = new Texture(dc, resources, pipeline, L"_DevelopmentAssets/Texture/CubeMaps/ValleySpecularHDR.dds");
	iblBRDF = new Texture(dc, resources, pipeline, L"_DevelopmentAssets/Texture/ibl_brdf_lut.png");

	cubeMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);
	irradianceMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 12);
	prefilteredEnvMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 13);
	iblBRDF->Set(TL_Graphics::E_SHADER_TYPE::PS, 15);

	CreateRenderTarget();
	CreateShader();
	CreateMesh();
}

CubeMapPass::~CubeMapPass()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(cubeMap);
	SAFE_DELETE(irradianceMap);
	SAFE_DELETE(prefilteredEnvMap);
	SAFE_DELETE(iblBRDF);
	SAFE_DELETE(rtts[0]);
	SAFE_DELETE(shaderPS);
}

void CubeMapPass::Set()
{
	//pipeline->SetSwapChainRenderTargetView(0);

	rtts[0]->SetRT(0);
	mesh->Set();
	shaderPS->Set();


	if (sourceTextures[0])
		sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);
	else
		cubeMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);

	if (sourceTextures[1])
		sourceTextures[1]->Set(TL_Graphics::E_SHADER_TYPE::PS, 12);
	else
		irradianceMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 12);

	if (sourceTextures[2])
		sourceTextures[2]->Set(TL_Graphics::E_SHADER_TYPE::PS, 13);
	else
		prefilteredEnvMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 13);

	if (sourceTextures[3])
		sourceTextures[3]->Set(TL_Graphics::E_SHADER_TYPE::PS, 15);
	else
		iblBRDF->Set(TL_Graphics::E_SHADER_TYPE::PS, 15);

}

void CubeMapPass::Execute()
{
	ClearRenderTargets();
	Set();
	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
}


void CubeMapPass::CreateRenderTarget()
{
	rtts[0] = new RenderTargetTexture(dc, resources, pipeline, 1.0f, 1.0f, "SkyBox");
}

void CubeMapPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SkyBoxPS.hlsl", "SkyBoxPS");
}

void CubeMapPass::CreateMesh()
{

	struct Vertex
	{
		float position[3];
	}
	vertices[8] = {
		//¾Õ¸é
	{-3,        +3,      -3},
	{+3,        +3,      -3},
	{-3,        -3,      -3},
	{+3,        -3,      -3},

	//µÞ¸é
	{-3,         +3,     +3},
	{+3,         +3,     +3},
	{-3,         -3,     +3},
	{+3,         -3,     +3}
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");


	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		//front
		0,1,2,
		1,3,2,
		//back
		5,4,7,
		4,6,7,

		//up
		4,5,0,
		5,1,0,
		//down
		2,3,6,
		3,7,6,

		//left
		4,0,6,
		0,2,6,
		//right
		1,5,3,
		5,7,3
	};
	mesh = new Mesh(dc, resources, pipeline, vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TL_SkyBoxVS.hlsl", "SkyBox");

}
