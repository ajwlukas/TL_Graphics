#include "pch_dx_11.h"
#include "Shader.h"

Shader::Shader(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName)
{
	//todo
	/*if (type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		resources->vertexShaders->Get(fileName);
	}

	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		dc->PSSetConstantBuffers(slot, 1, constantBuffer->buffer);

		currentConstantBuffersPS[slot] = constantBuffer;
	}*/
}

Shader::~Shader()
{
}
