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
	LightAdaptionPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~LightAdaptionPass();

	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	virtual void Execute() override;

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;

	Shader* downSampler3x3;

	Shader* downSamplerLuminance;

	SamplerPass* downSamplers[downSamplingCount];

private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};