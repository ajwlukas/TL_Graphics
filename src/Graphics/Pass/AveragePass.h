#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

#include "Pass/SamplerPass.h"

class AveragePass : public IRenderPass
{
public:
	AveragePass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~AveragePass();

	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	virtual void Execute() override;

private:
	Shader* shaderPS;

private:
	void CreateShader();
};