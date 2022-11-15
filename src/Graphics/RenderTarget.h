#pragma once

#include <d3d11.h>

#include "Resources.h"

class Pipeline;

class RenderTarget
{
public:
	RenderTarget(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	virtual ~RenderTarget();

	virtual void Set(UINT slot) ;
	
protected:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11RenderTargetView> rtv;
};