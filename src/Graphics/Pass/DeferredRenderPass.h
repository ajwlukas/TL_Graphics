#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class DeferredRenderPass : public IRenderPass
{
public:
	DeferredRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~DeferredRenderPass();

	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	virtual void Execute() override;

	//virtual void ClearRenderTargets() override;

private : 
	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};