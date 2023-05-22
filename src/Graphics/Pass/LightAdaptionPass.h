#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

#include "Pass/SamplerPass.h"

const unsigned int downSamplingCount = 6;

class LightAdaptionPass : public IRenderPass
{
public:
	LightAdaptionPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~LightAdaptionPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	void SetMiddleGrey(float _middleGrey) {
		m_Figure.middleGrey = _middleGrey;
		figureBuffer->Update(&m_Figure, sizeof(m_Figure));
	}

private:

	Shader* shaderPS;


	ConstantBuffer* figureBuffer;
	__declspec(align(16))struct Figure
	{
		float middleGrey = 0.5f;

	}m_Figure;

private:
	void CreateRenderTarget();

	void CreateShader();
};