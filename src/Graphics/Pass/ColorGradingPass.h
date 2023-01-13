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

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	virtual void ClearRenderTargets() override;

private:
	OnResizeNotice* resizeNotice;
	RenderTargetTexture* rtt;

	Shader* shaderPS;

	Texture* LUT;

	__declspec(align(16)) struct LUT_Info
	{
		UINT width;
		UINT height;
	}lut_info;

	ConstantBuffer* LUT_Info_buffer;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};