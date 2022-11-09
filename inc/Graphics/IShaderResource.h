#pragma once

#include "Export.h"

#include "ShaderType.h"

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT IShaderResource
	{
	public:
		virtual void Set() = 0;

	};
}