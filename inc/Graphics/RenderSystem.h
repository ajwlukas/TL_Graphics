#pragma once

#include "Export.h"

#include "IMesh.h"
#include "IMaterial.h"
#include "IVertex.h"

#include "IConstantBuffer.h"
#include "ICamera.h"

#include "IShaderResource.h"

#include "IRenderTargetTexture.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT RenderSystem
	{
	public:
		static void Create();
		static void Delete();
		static RenderSystem* Get();

		virtual HRESULT Init() = 0;

		virtual void Clear() = 0;
		virtual void Draw() = 0;
		virtual void DrawInstanced(UINT numInstance) = 0;
		virtual void Present() = 0;

		virtual IMesh* CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, std::wstring fileName) = 0;

		virtual IMaterial* CreateMaterial(std::wstring fileName, const MaterialDesc& desc = MaterialDesc()) = 0;

		virtual IConstantBuffer* CreateConstantBuffer( void* data, size_t dataSize) = 0;

		virtual ICamera* CreateCamera() = 0;

		virtual IShaderResource* CreateTexture( std::wstring fileName) = 0;

		virtual IShaderResource* CreateTextureBuffer( void* data, size_t dataSize) = 0;

		virtual IRenderTargetTexture* CreateRenderTargetTexture(float widthRatio = 1.0f, float heightRatio = 1.0f) = 0;


		void Return(void* address);//Create한거 지우는 거

		virtual void UpdateWindowSize(UINT width, UINT height) = 0;

		//바로 다음 드로우콜에만 적용된다.
		virtual void DrawWireOnce() = 0;
		virtual void DrawSolidOnce() = 0;

		virtual void SetSolidModeAsDefualt() = 0;
		virtual void SetWireModeAsDefualt() = 0;

		virtual ID3D11Device* GetDevice() = 0;
		virtual ID3D11DeviceContext* GetDeviceContext() = 0;

	};

}