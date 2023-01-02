#pragma once

#include <d3d11.h>

#include "Resources.h"

class Pipeline;

class IShaderPass
{
public:
	IShaderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~IShaderPass();

	virtual void Set() = 0;

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

};