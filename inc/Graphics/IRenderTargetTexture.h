#pragma once

#include "Export.h"

#include "ShaderType.h"

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT IRenderTargetTexture
	{
	public:
		virtual void SetRT(UINT slot) = 0;
		virtual void SetT(TL_Graphics::E_SHADER_TYPE type, UINT slot) = 0;
	};
}