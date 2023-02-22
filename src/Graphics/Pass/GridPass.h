#pragma once

#include "IRenderPass.h"

#include "OnResize.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

/// <summary>
/// Deferred�� �ʿ��� �ؽ��ĸ� �̾��ִ� �� �ʿ��� �͵� �� ����
/// </summary>

class GridPass : public IRenderPass
{
public:
	GridPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~GridPass();


	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;
	virtual void Execute() override;


private:

	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();

};
