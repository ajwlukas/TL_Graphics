#pragma once

#include <string>
#include "Export.h"

#include "Math\TL_Math.h"

namespace TL_Graphics
{
	struct MaterialDesc
	{
		TL_Math::Vector4 ambient;
		TL_Math::Vector4 diffuse;
		TL_Math::Vector4 specular;

		std::wstring diffuseFileName;
		std::wstring normalFileName;
	};

	class AJW_GRAPHICS_DLLEXPORT IMaterial
	{
	public:
		virtual void Set() = 0;

	};
}