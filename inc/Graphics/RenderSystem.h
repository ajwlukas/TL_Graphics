#pragma once

#include "Export.h"

#include "IMesh.h"
#include "IMaterial.h"
#include "IVertex.h"

#include "IConstantBuffer.h"
#include "ICamera.h"

#include "IShaderResource.h"

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
		virtual void Present() = 0;

		virtual IMesh* CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, std::wstring fileName) = 0;

		virtual IMaterial* CreateMaterial(std::wstring fileName, const MaterialDesc& desc = MaterialDesc()) = 0;

		virtual IConstantBuffer* CreateConstantBuffer(UINT slot, TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize) = 0;

		virtual ICamera* CreateCamera() = 0;

		virtual IShaderResource* CreateTexture(UINT slot, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName) = 0;

		void Return(void* address);//Create한거 지우는 거

		virtual void UpdateWindowSize(UINT width, UINT height) = 0;
	};
}