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

#include "Pipeline.h"


class  DX11Renderer : public TL_Graphics::RenderSystem
{
public:
	DX11Renderer();
	virtual ~DX11Renderer();

	virtual HRESULT Init() override;

	virtual void Clear()override;
	virtual void Draw() override;
	virtual void Present()override;

	virtual Mesh* CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, std::wstring fileName) override;

	virtual Material* CreateMaterial(std::wstring fileName, const TL_Graphics::MaterialDesc& desc = TL_Graphics::MaterialDesc()) override;

	virtual ConstantBuffer* CreateConstantBuffer(UINT slot, TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize) override;

	virtual Camera* CreateCamera() override;

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

};
