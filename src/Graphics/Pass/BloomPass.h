#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

class BloomPass : public IRenderPass
{
public:
	BloomPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~BloomPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	virtual void ClearRenderTargets() override;

private:
	OnResizeNotice* resizeNotice;

	RenderTargetTexture* rtt[4];

	Shader* shaderPS;

private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};