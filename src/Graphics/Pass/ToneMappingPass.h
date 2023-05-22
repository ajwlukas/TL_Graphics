#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

#include "SamplerPass.h"

#include "GaussianBlurPass.h"

#include "AccumulatorPass.h"

#include "LightPass.h"

class ToneMappingPass : public IRenderPass
{
public:
	ToneMappingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~ToneMappingPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	void SetMaxWhite(float _maxWhite) {
		m_Figure.maxWhite = _maxWhite;
		figureBuffer->Update(&m_Figure, sizeof(m_Figure));
	}

private:
	Shader* shaderPS;


	ConstantBuffer* figureBuffer;
	__declspec(align(16))struct Figure
	{
		float maxWhite = 0.5f;

	}m_Figure;

private:
	void CreateRenderTargets(OnResizeNotice* resizeNotice);
	void DeleteRenderTargets();

	void CreateShader();
};