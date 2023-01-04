#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class DeferredRenderPass : public IRenderPass
{
public:
	DeferredRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~DeferredRenderPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

private : 
	RenderTargetTexture* rtt;

	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};