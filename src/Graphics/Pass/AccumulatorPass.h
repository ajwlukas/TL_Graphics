#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

#include "SamplerPass.h"

class AccumulatorPass : public IRenderPass
{
public:
	AccumulatorPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~AccumulatorPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;


private:

	Shader* shaderPS;

	SamplerPass* samplerPass;
private:
	void CreateRenderTarget();

	void CreateShader();
};