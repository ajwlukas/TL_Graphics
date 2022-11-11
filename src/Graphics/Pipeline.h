#pragma once

#include <d3d11.h>
#include "Resources.h"

#include "OnResize.h"

#include "Mesh.h"
#include "Material.h"

#include "ConstantBuffer.h"
#include "ShaderResource.h"

/// <summary>
/// dc->DrawIndexed() 호출 시점이 애매해져서 만든 클래스
/// 
/// this class represent Device Context
/// 
/// 221018 Ssreppa
/// </summary>

class Pipeline : public OnResize_Observer
{
public:
	Pipeline(ID3D11DeviceContext* dc, IDXGISwapChain* swapChain, OnResizeNotice* resizeNotice, Resources* resources);
	~Pipeline();

	void Init(UINT width, UINT height);

	// OnResize_Observer을(를) 통해 상속됨
	virtual void OnResize(uint32_t width, uint32_t height) override;


	void Clear(float color[4]);

	void SetCurrentRasterState(Resource<ID3D11RasterizerState> state);
	void SetCurrentDepthStencilState(Resource<ID3D11DepthStencilState> state);
	void SetCurrentBlendState(Resource<ID3D11BlendState> state);
	void SetCurrentSamplerState(Resource<ID3D11SamplerState> state);

	void SetViewPort();

	void SetConstantBuffer(ConstantBuffer* constantBuffer);
	void SetShaderResource(ShaderResource* shaderResource);

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);

	void SetRenderTarget(Resource<ID3D11RenderTargetView> rtv);//todo : depthStencilView도 해줘야하는가? 아직 depthStencilView가 뭔지 잘 모르겠다.
	void SetRenderTarget_SwapChain();//스왑체인의 렌더타겟을 꽂는다.

	void Draw();
	void DrawInstanced(UINT numInstance);
private:
	ID3D11DeviceContext* dc;
	IDXGISwapChain* swapChain;
	Resources* resources;

	Mesh* currentMesh;
	Material* currentMaterial;

	ConstantBuffer* currentConstantBuffersVS[255];
	ConstantBuffer* currentConstantBuffersPS[255];

	ShaderResource* currentShaderResourceVS[255];
	ShaderResource* currentShaderResourcePS[255];

	Resource<ID3D11RasterizerState> currentRasterState;
	Resource<ID3D11DepthStencilState> currentDepthStencilState;
	Resource<ID3D11BlendState> currentBlendState;

	/// <summary>
	/// //////////////////////////////////////////////
	/// </summary>
	Resource<ID3D11RasterizerState> defaultRasterState;
	Resource<ID3D11DepthStencilState> defaultDepthStencilState;
	Resource<ID3D11BlendState> defaultBlendState;
	Resource<ID3D11SamplerState> defualtSamplerState;

	Resource<ID3D11DepthStencilView> depthStencilView;
	Resource<ID3D11Texture2D> depthStencilBuffer;

	Resource<ID3D11RenderTargetView> swapChainRtv;//swapChain에 present할 용도의 렌더타겟
	D3D11_VIEWPORT viewPort;

	void CreateDefaultStates();

	void ResizeSwapChainRtv(UINT width, UINT height);//aka Create
	void ResizeDepthStencilView(UINT width, UINT height);	//aka Create
	void ResizeViewPort(UINT width, UINT height);//aka Create
};