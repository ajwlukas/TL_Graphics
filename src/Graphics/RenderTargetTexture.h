#pragma once

#include "IRenderTargetTexture.h"

#include "ShaderResource.h"
#include "RenderTarget.h"
#include "OnResize.h"

#include "Texture.h"

class RenderTargetTexture : public TL_Graphics::IRenderTargetTexture,  public Texture, public OnResize_Observer,  public RenderTarget
{
public:
	RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT width, UINT height);
	RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float widthRatio = 1.0f, float heightRatio = 1.0f);
	virtual ~RenderTargetTexture();

	// IRenderTargetTexture��(��) ���� ��ӵ�

	virtual void SetRT(UINT slot) override;
	//todo : naming �� �̷������� �ٲ� ���� �� �򰥸���
	virtual void SetT(TL_Graphics::E_SHADER_TYPE type, UINT slot) override;

	virtual void Clear(TL_Math::Vector4 color = { 0,0,0,0 }) override;

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11Texture2D> texture;


	bool isBasedWindowSize = true;
	float widthRatio, heightRatio;
	// OnResize_Observer��(��) ���� ��ӵ�
	virtual void OnResize(uint32_t width, uint32_t height) override;




};