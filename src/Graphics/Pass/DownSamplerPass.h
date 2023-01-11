#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class DownSamplerPass : public IRenderPass
{
public:
	DownSamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float widthRatio = 1.0f, float heightRatio = 1.0f);
	~DownSamplerPass();

	// IRenderPass��(��) ���� ��ӵ�
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


	float widthRatio, heightRatio;
};