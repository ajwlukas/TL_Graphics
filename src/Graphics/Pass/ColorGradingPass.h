#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

class ColorGradingPass : public IRenderPass
{
public:
	ColorGradingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~ColorGradingPass();

	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	virtual void Execute() override;

	virtual void ClearRenderTargets() override;

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;

	Texture* LUT;

private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};