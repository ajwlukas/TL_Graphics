#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class DownSamplerPass : public IRenderPass
{
public:
	DownSamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~DownSamplerPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	virtual void ClearRenderTargets() override;

private:
	RenderTargetTexture* rtt;

	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};