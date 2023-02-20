#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class SamplerPass : public IRenderPass
{
public:
	SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float widthRatio = 1.0f, float heightRatio = 1.0f, Shader* shaderPS = nullptr);

	SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT width, UINT height, Shader* shaderPS = nullptr);

	~SamplerPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	virtual void ClearRenderTargets() override;

	void SetSourceTexture(Texture* texture);
	Texture* GetDestTexture() { return rtt; }

	//둘 중 하나만 동작함, Ratio는 화면 비율, Size = 픽셀 수
	void SetRatio(float sizeX, float sizeY);//(0.0, 1.0]
	void SetSize(UINT sizeX, UINT sizeY);

private:
	OnResizeNotice* resizeNotice;
	RenderTargetTexture* rtt;

	Texture* sourceTexture;

	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);
	void CreateShader();

	bool isShaderCreatedHere = false;
	bool isBasedWindowSize;
	float widthRatio = 0, heightRatio = 0;
	UINT width = 0, height = 0;

	D3D11_VIEWPORT viewPort;

	void DescViewPort();
	void ResizeViewport();
};