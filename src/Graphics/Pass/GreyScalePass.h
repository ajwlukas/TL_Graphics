#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

#include "Pass/SamplerPass.h"

class GreyScalePass : public IRenderPass
{
public:
	GreyScalePass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~GreyScalePass();

	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	virtual void Execute() override;

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;

private:
	void CreateShader();
};