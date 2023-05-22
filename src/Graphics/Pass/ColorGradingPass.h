#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

class ColorGradingPass : public IRenderPass
{
public:
	ColorGradingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~ColorGradingPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	void SetLUT(TL_Graphics::ITexture* texture) { LUT = texture; }

private:

	Shader* shaderPS;

	TL_Graphics::ITexture* LUT = nullptr;

private:
	void CreateRenderTarget();

	void CreateShader();
};