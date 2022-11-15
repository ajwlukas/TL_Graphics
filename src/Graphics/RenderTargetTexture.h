#pragma once

#include "ShaderResource.h"
#include "OnResize.h"

class RenderTargetTexture : public ShaderResource, public OnResize_Observer
{
public:
	RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, UINT width, UINT height);
	~RenderTargetTexture();


private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11Texture2D> texture;
	Resource<ID3D11RenderTargetView> rtv;


	// OnResize_Observer��(��) ���� ��ӵ�
	virtual void OnResize(uint32_t width, uint32_t height) override;

};