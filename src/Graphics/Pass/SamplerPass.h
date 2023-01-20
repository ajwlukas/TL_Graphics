#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class SamplerPass : public IRenderPass
{
public:
	SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float widthRatio = 1.0f, float heightRatio = 1.0f, Shader* shaderPS = nullptr);

	SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT width, UINT height, Shader* shaderPS = nullptr);

	~SamplerPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	virtual void ClearRenderTargets() override;

private:
	OnResizeNotice* resizeNotice;
	RenderTargetTexture* rtt;

	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();

	bool isShaderCreatedHere = false;
	bool isBasedWindowSize;
	float widthRatio = 0, heightRatio = 0;
	UINT width = 0, height = 0;
};