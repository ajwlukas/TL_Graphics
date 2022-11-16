#pragma once

#include <functional>
#include <d3d11.h>
#include "Resources.h"

#include "OnResize.h"

#include "Mesh.h"
#include "Material.h"

#include "ConstantBuffer.h"
#include "ShaderResource.h"

#include "RenderTarget.h"
#include "SwapChainRenderTarget.h"

/// <summary>
/// dc->DrawIndexed() 호출 시점이 애매해져서 만든 클래스
/// 
/// this class represent Device Context
/// 
/// 221018 Ssreppa
/// </summary>

constexpr UINT MAX_RENDERTARGET = 8;

class Pipeline : public OnResize_Observer
{
public:
	Pipeline(ID3D11DeviceContext* dc, IDXGISwapChain* swapChain, OnResizeNotice* resizeNotice, Resources* resources);
	~Pipeline();

	void Init(UINT width, UINT height);

	// OnResize_Observer을(를) 통해 상속됨
	virtual void OnResize(uint32_t width, uint32_t height) override;


	void Clear(float color[4]);
	void ClearRenderTarget(RenderTarget* renderTarget, TL_Math::Vector4 color);

	void SetCurrentRasterState(Resource<ID3D11RasterizerState> state);
	void SetCurrentDepthStencilState(Resource<ID3D11DepthStencilState> state);
	void SetCurrentBlendState(Resource<ID3D11BlendState> state);
	void SetCurrentSamplerState(Resource<ID3D11SamplerState> state);

	void SetViewPort();

	void SetConstantBuffer(ConstantBuffer* constantBuffer, TL_Graphics::E_SHADER_TYPE type,
		UINT slot);
	void SetShaderResource(ShaderResource* shaderResource, TL_Graphics::E_SHADER_TYPE type,
		UINT slot);

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);

	void SetRenderTarget(RenderTarget* rtv, UINT slot);//todo : depthStencilView도 해줘야하는가? 아직 depthStencilView가 뭔지 잘 모르겠다.

	void SetSwapChainRenderTargetView(UINT slot = 0);

	void UnSetAllRenderTargets();

	void UnSetRenderTarget(UINT slot);//렌더타겟을 빼준다.

	void Draw();
	void DrawInstanced(UINT numInstance);
private:
	ID3D11DeviceContext* dc;
	IDXGISwapChain* swapChain; friend class SwapChainRenderTarget;
	Resources* resources;

	Mesh* currentMesh;
	Material* currentMaterial;

	ConstantBuffer* currentConstantBuffersVS[255];
	ConstantBuffer* currentConstantBuffersPS[255];

	ShaderResource* currentShaderResourceVS[255];
	ShaderResource* currentShaderResourcePS[255];

	RenderTarget* currentRenderTarget[MAX_RENDERTARGET];

	Resource<ID3D11RasterizerState> currentRasterState;
	Resource<ID3D11DepthStencilState> currentDepthStencilState;
	Resource<ID3D11BlendState> currentBlendState;

	/// <summary>
	/// //////////////////////////////////////////////
	/// </summary>
	Resource<ID3D11RasterizerState> defaultRasterState;
	Resource<ID3D11RasterizerState> wireFrameRasterState;
	Resource<ID3D11RasterizerState> solidRasterState;


	Resource<ID3D11DepthStencilState> defaultDepthStencilState;
	Resource<ID3D11BlendState> defaultBlendState;
	Resource<ID3D11SamplerState> defualtSamplerState;

	Resource<ID3D11DepthStencilView> depthStencilView;
	Resource<ID3D11Texture2D> depthStencilBuffer;

	SwapChainRenderTarget* swapChainRtv;//swapChain에 present할 용도의 렌더타겟

	ID3D11RenderTargetView* renderTargets[MAX_RENDERTARGET] = {};
	D3D11_VIEWPORT viewPort;

	void CreateDefaultStates();

	//void ResizeSwapChainRtv(UINT width, UINT height);//aka Create
	void ResizeDepthStencilView(UINT width, UINT height);	//aka Create
	void ResizeViewPort(UINT width, UINT height);//aka Create

public:
	void DrawWireOnce();
	void DrawSolidOnce();

	void SetSolidModeAsDefualt();
	void SetWireModeAsDefualt();

private:
	vector<function<void()>> reservations;//calls after one drawcall
};