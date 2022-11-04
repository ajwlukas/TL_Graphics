#pragma once

#include <d3d11.h>
#include "Resources.h"

#include "Mesh.h"
#include "Material.h"

/// <summary>
/// dc->DrawIndexed() 호출 시점이 애매해져서 만든 클래스
/// 
/// 221018 Ssreppa
/// </summary>

class Pipeline
{
public:
	Pipeline(ID3D11DeviceContext* dc, Resources* resources);
	~Pipeline();

	void SetCurrentRasterState(Resource<ID3D11RasterizerState> state);
	void SetCurrentDepthStencilState(Resource<ID3D11DepthStencilState> state);
	void SetCurrentBlendState(Resource<ID3D11BlendState> state);

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);

	void Draw();
private:
	ID3D11DeviceContext* dc;
	Resources* resources;

	Mesh* currentMesh;
	Material* currentMaterial;

	Resource<ID3D11RasterizerState> currentRasterState;
	Resource<ID3D11DepthStencilState> currentDepthStencilState;
	Resource<ID3D11BlendState> currentBlendState;

	/// <summary>
	/// //////////////////////////////////////////////
	/// </summary>
	Resource<ID3D11RasterizerState> defaultRasterState;
	Resource<ID3D11DepthStencilState> defaultDepthStencilState;
	Resource<ID3D11BlendState> defaultBlendState;

	Resource<ID3D11DepthStencilView> depthStencilView;
	Resource<ID3D11Texture2D> depthStencilBuffer;

	Resource<ID3D11RenderTargetView> swapChainRtv;//swapChain에 present할 용도의 렌더타겟

	void CreateDefaultStates();

};