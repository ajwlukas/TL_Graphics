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

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	virtual void ClearRenderTargets() override;

private:
	OnResizeNotice* resizeNotice;
	RenderTargetTexture* rtt;

	Shader* shaderPS;

	Shader* downSampler3x3;

	Shader* downSamplerLuminance;

	SamplerPass* downSamplers[downSamplingCount];

private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};