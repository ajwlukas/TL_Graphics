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

	};
}