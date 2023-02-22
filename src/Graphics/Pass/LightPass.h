#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"


class LightPass : public IRenderPass
{
public:
	LightPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~LightPass();

	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	virtual void Execute() override;

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;


private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};