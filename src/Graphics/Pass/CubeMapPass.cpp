#include "pch_dx_11.h"
#include "CubeMapPass.h"

#include "Pipeline.h"

#include "Vertex.h"

CubeMapPass::CubeMapPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, std::wstring fileName)
	:IRenderPass(dc, resources, pipeline)
{
	texture = new Texture(dc, resources, pipeline, fileName);

	texture->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);

	CreateRenderTarget(resizeNotice);
	CreateShader();
	CreateMesh();
}

CubeMapPass::~CubeMapPass()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(texture);
	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void CubeMapPass::Set()
{
	//rtt->SetRT(0);
    pipeline->SetSwapChainRenderTargetView(0);
    mesh->Set();
	shaderPS->Set();
}

void CubeMapPass::Execute()
{
	ClearRenderTargets();
	Set();
	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, 14);
}

void CubeMapPass::ClearRenderTargets()
{
	rtt->Clear();
}

void CubeMapPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice);
}

void CubeMapPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SkyBoxPS.hlsl");
}

void CubeMapPass::CreateMesh()
{

    struct Vertex
    {
        float position[3];
    }
    vertices[8] = {
        //�ո�
    {-3,        +3,      -3},
    {+3,        +3,      -3},
    {-3,        -3,      -3},
    {+3,        -3,      -3},

    //�޸�
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
    mesh = new Mesh(resources, pipeline, vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TL_SkyBoxVS.hlsl");

}