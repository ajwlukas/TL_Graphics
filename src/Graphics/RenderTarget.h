#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "Math\TL_Math.h"

class Pipeline;

class RenderTarget
{
public:
	RenderTarget(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::string debugName = "");
	virtual ~RenderTarget();

	virtual void Set(UINT slot) ;
	void SetOnce(UINT slot);
	
	void Clear(TL_Math::Vector4 color = { 0,0,0,0 });

protected:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11RenderTargetView> rtv;
};