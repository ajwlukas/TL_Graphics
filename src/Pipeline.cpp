#include "pch_dx_11.h"
#include "Pipeline.h"

Pipeline::Pipeline(ID3D11DeviceContext* dc, Resources* resources)
	:dc(dc), currentMaterial(nullptr), currentMesh(nullptr)
	,resources(resources)
{
	CreateDefaultStates();

	 SetCurrentRasterState(defaultRasterState);
	 SetCurrentDepthStencilState(defaultDepthStencilState);
	 SetCurrentBlendState(defaultBlendState);
}

Pipeline::~Pipeline()
{
}

void Pipeline::SetMesh(Mesh* mesh)
{
	dc->IASetInputLayout(mesh->inputLayout);
	dc->IASetPrimitiveTopology(mesh->topology);
	dc->IASetVertexBuffers(0, 1, mesh->vertexBuffer, &mesh->vertexDataSize, &mesh->offset);
	dc->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->VSSetShader(mesh->shader, 0, 0);

	currentMesh = mesh;
}

void Pipeline::SetCurrentRasterState(Resource<ID3D11RasterizerState> state)
{
	dc->RSSetState(state);
	currentRasterState = state;
}

void Pipeline::SetMaterial(Material* material)
{
	currentMaterial = material;
}

void Pipeline::SetCurrentBlendState(Resource<ID3D11BlendState> state)
{
	dc->OMSetBlendState(state, NULL, 0xFF);
	currentBlendState = state;
}

void Pipeline::SetCurrentDepthStencilState(Resource<ID3D11DepthStencilState> state)
{
	dc->OMSetDepthStencilState(state, 1);
	currentDepthStencilState = state;
}

void Pipeline::Draw()
{
	dc->DrawIndexed(currentMesh->GetIndexCount(), 0, 0);
}

void Pipeline::CreateDefaultStates()
{
	resources->rasterStates->GetDefault(defaultRasterState);
	resources->depthStencilStates->GetDefault(defaultDepthStencilState);
	resources->blendStates->GetDefault(defaultBlendState);
}
