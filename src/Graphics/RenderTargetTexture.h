#pragma once

#include "ShaderResource.h"
#include "OnResize.h"

class RenderTargetTexture : public ShaderResource, public OnResize_Observer
{
public:
	RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT width, UINT height);
	~RenderTargetTexture();

	void Set(TL_Graphics::E_SHADER_TYPE type,
		UINT slot);

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11Texture2D> texture;
	Resource<ID3D11RenderTargetView> rtv;


	bool isBasedWindowSize = true;
	// OnResize_Observer을(를) 통해 상속됨
	virtual void OnResize(uint32_t width, uint32_t height) override;

};