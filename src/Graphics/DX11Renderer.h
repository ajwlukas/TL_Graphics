#pragma once

#include "RenderSystem.h"

#include <d3d11.h>
#include <vector>

#include "OnResize.h"

#include "Resource.h"
#include "Resources.h"

#include "Mesh.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Camera.h"

#include "Texture.h"
#include "TextureBuffer.h"

#include "RenderTargetTexture.h"

#include "Light.h"

#include "Pipeline.h"


#include "ScreenMesh.h"

#include "Pass/GBufferRenderPass.h"

#include "Pass/DeferredRenderPass.h"

#include "Pass/GridPass.h"

#include "Pass/DownSamplerPass.h"

#include "Pass/FinalPass.h"


class  DX11Renderer : public TL_Graphics::RenderSystem
{
public:
	DX11Renderer();
	virtual ~DX11Renderer();

	virtual HRESULT Init() override;

	// RenderSystem을(를) 통해 상속됨
	virtual void PreRender() override;

	virtual void Clear()override;
	virtual void Draw() override;
	virtual void DrawInstanced(UINT numInstance) override;
	virtual void Present()override;

	virtual void PostRender() override;

	virtual Shader* CreateShader(TL_Graphics::E_SHADER_TYPE type, std::wstring fileName) override;

	virtual Mesh* CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, std::wstring fileName) override;

	virtual Mesh* CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, TL_Graphics::E_MESH_TYPE meshType) override;

	virtual Material* CreateMaterial( const TL_Graphics::MaterialDesc& desc = TL_Graphics::MaterialDesc()) override;

	virtual ConstantBuffer* CreateConstantBuffer( void* data, size_t dataSize) override;

	virtual Camera* CreateCamera() override;

	virtual Texture* CreateTexture(std::wstring fileName) override;

	virtual TextureBuffer* CreateTextureBuffer(void* data, size_t dataSize) override;

	virtual RenderTargetTexture* CreateRenderTargetTexture(float widthRatio = 1.0f, float heightRatio = 1.0f) override;

	virtual void UnSetAllRenderTargets() override;
	virtual void UnSetRenderTarget(UINT slot) override;

	virtual void UpdateWindowSize(UINT width, UINT height) override;

private:
	HWND hWnd;

	HRESULT CreateDeviceAndSwapChain();
	ID3D11Device* device;
	ID3D11DeviceContext* dc;
	IDXGISwapChain* swapChain;

	OnResizeNotice onResizeNotice;

	Resources* resources;//this class represent device
	Pipeline* pipeline;//this class represent device context


					   // RenderSystem을(를) 통해 상속됨
	virtual void DrawWireOnce() override;

	virtual void DrawSolidOnce() override;

	virtual void SetSolidModeAsDefualt() override;

	virtual void SetWireModeAsDefualt() override;

	virtual void SetSwapChainRenderTargetView(UINT slot = 0) override;


	// RenderSystem을(를) 통해 상속됨
	virtual ID3D11Device* GetDevice() override;

	virtual ID3D11DeviceContext* GetDeviceContext() override;

	Light* lights;

	virtual void BeginSetLight() override;

	virtual void SetLight(TL_Graphics::DirectionalLight* light) override;

	virtual void SetLight(TL_Graphics::PointLight* light) override;

	virtual void SetLight(TL_Graphics::SpotLight* light) override;

	virtual void EndSetLight() override;


	GBufferRenderPass* gBufferRenderPass;

	ScreenMesh* screenMesh;

	DeferredRenderPass* deferredRenderPass;

	GridPass* gridPass;

	DownSamplerPass* downSamplerPass;

	FinalPass* finalPass;
};
