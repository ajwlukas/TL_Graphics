#pragma once

#include "IVertex.h"

class TL_Graphics::VertexAttribute::Impl
{
public:
	void* GetVerticesData() { return verticesData.data(); }
	UINT GetVerticesSize() { return verticesData.size() * sizeof(DWORD); }
	UINT GetVertexSize() { return stride; }

	D3D11_INPUT_ELEMENT_DESC* GetDescsData() { return descs.data(); }
	UINT GetDescsSize() { return descs.size(); }

private:
	friend class TL_Graphics::VertexAttribute;
	std::vector<DWORD> verticesData;//���� ���ؽ����� ������
	UINT memIndex;

	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	UINT stride;//���ؽ� �ϳ��� ũ��

	std::string semanticString;
	//static ShaderNameFinder shaderFinder;//��ǲ ���п� ���� �˸��� �⺻ ���̴��� �̸��� �ְ�
};
