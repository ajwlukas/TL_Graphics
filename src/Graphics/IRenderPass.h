#pragma once

#include <d3d11.h>

#include "Resources.h"

class Pipeline;

class IRenderPass
{
public:
	IRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~IRenderPass();

	virtual void Set() = 0;

protected:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

};