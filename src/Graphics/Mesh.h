#pragma once

#include <d3d11.h>
#include <string>

#include "IMesh.h"
#include "Resources.h"
#include "Vertex.h"

#include "Shader.h"

/// <summary>
/// ���ؽ�, �ε���, ��ǲ���̾ƿ� ������ ������ �ִ� ������Ʈ
/// 
/// written by Ssreppa 22.06.22
/// </summary>

class Pipeline;

 class  Mesh : public TL_Graphics::IMesh
{
public:
	Mesh(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount,
		std::wstring vsFileName = L"UVLightVertex.hlsl", std::string debugNameShader = "", std::string debugName = "", D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual ~Mesh();
	virtual void Set() override;

	const UINT& GetIndexCount() const { return indexCount; }

	void SetShader(std::wstring vsFileName) {
		this->vsFileName = vsFileName;
		resources->inputLayouts->Get(inputLayout, desc, descSize, vsFileName);
		//shader = resources->vertexShaders->Get(vsFileName);
	}

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	std::wstring vsFileName;
	Resource<ID3D11InputLayout> inputLayout;
	D3D_PRIMITIVE_TOPOLOGY topology;

	UINT vertexDataSize;
	UINT indexCount;
	UINT offset = 0;
	Resource<ID3D11Buffer> vertexBuffer;
	Resource<ID3D11Buffer> indexBuffer;
	//Resource<ID3D11VertexShader> shader;

	D3D11_INPUT_ELEMENT_DESC* desc;
	UINT descSize;

	Shader* shader;
};