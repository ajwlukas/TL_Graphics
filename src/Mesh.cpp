#include "pch_dx_11.h"
#include "Mesh.h"

#include "Pipeline.h"

Mesh::Mesh(Resources* resources, Pipeline* pipeline, TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, wstring vsFileName, D3D_PRIMITIVE_TOPOLOGY topology)
	:vertexDataSize(vertexSet.pimpl->GetVertexSize()) 
	,indexCount(indexCount)
	, vsFileName(vsFileName)
	, topology(topology)
	, resources(resources)
	,pipeline(pipeline)
{
	desc = vertexSet.pimpl->GetDescsData();
	descSize = vertexSet.pimpl->GetDescsSize();

	//inputLayout
	resources->inputLayouts->Get(inputLayout, desc, descSize, vsFileName);

	//shader
	shader = resources->vertexShaders->Get(vsFileName);

	//vertexBuffer;
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = vertexSet.pimpl->GetVerticesSize();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertexSet.pimpl->GetVerticesData();

	resources->buffers->Create(vertexBuffer, vbd, &vinitData);

	//index Buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indexData;

	resources->buffers->Create(indexBuffer, ibd, &iinitData);
}

Mesh::~Mesh()
{
}

void Mesh::Set()
{
	pipeline->SetMesh(this);
}
