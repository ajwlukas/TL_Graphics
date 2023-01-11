#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "ShaderResourceSlotPS.h"

class Pipeline;

class IRenderPass
{
public:
	IRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~IRenderPass();

	virtual void Set() = 0;

	virtual void Execute() {};

	virtual void ClearRenderTargets() = 0;

protected:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

};