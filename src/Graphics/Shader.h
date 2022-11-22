#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "ShaderType.h"

class Pipeline;

class Shader
{
public:
	Shader(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName);
	virtual ~Shader();

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

protected:
	Resource<ID3D11DeviceChild> shader;
};