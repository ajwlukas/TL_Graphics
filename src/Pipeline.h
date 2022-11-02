#pragma once

#include <d3d11.h>
#include "Resources.h"

#include "Mesh.h"
#include "Material.h"

/// <summary>
/// dc->DrawIndexed() ȣ�� ������ �ָ������� ���� Ŭ����
/// 
/// 221018 Ssreppa
/// </summary>

class Pipeline
{
public:
	Pipeline(ID3D11DeviceContext* dc);
	~Pipeline();

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);

	void Draw();
private:
	ID3D11DeviceContext* dc;
	Resources* resources;

	Mesh* lastMesh;
	Material* lastMaterial;
};