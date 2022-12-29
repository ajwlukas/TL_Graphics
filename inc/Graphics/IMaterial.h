#pragma once

#include <string>
#include "Export.h"

#include "Math\TL_Math.h"


namespace TL_Graphics
{
	struct MaterialDesc
	{
		std::wstring albedoMapFileName = L"";
		std::wstring metallicMapFileName = L"";
		std::wstring roughnessMapFileName = L"";
	};

	class AJW_GRAPHICS_DLLEXPORT IMaterial
	{
	public:
		virtual void Set(UINT albdeoMapSlot = 0, UINT metallicMapSlot = 1, UINT roughnessMapSlot = 2) = 0;
		
		virtual void SetAlbedo(class IShaderResource*) = 0;
		virtual void SetMetallic(class IShaderResource*) = 0;
		virtual void SetRoughness(class IShaderResource* ) = 0;

	};
}