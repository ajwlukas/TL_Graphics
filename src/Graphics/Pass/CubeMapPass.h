#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Mesh.h"

#include "Shader.h"

class CubeMapPass : public IRenderPass
{
public:
	CubeMapPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~CubeMapPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;


private:
	Mesh* mesh;

	Texture* cubeMap;
	Texture* irradianceMap;
	Texture* prefilteredEnvMap;
	Texture* iblBRDF;

	Shader* shaderPS;
private:
	void CreateRenderTarget();

	void CreateShader();

	void CreateMesh();
};