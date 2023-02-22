#pragma once

#include "IRenderPass.h"

#include "OnResize.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

/// <summary>
/// Deferred�� �ʿ��� �ؽ��ĸ� �̾��ִ� �� �ʿ��� �͵� �� ����
/// written by Ssreppa 230110
/// </summary>

class GBufferRenderPass : public IRenderPass
{
public:
	GBufferRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~GBufferRenderPass();


	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	void SetGBuffers();



private:
	Shader* shaderPS;

	Resource<ID3D11BlendState> blendState;//Deferred�� �ʿ��� GBuffer�� ���� �� ���� BlendState
private:
	void CreateRenderTargets(OnResizeNotice* resizeNotice);

	void CreateBlendStates();

	void CreateShader();

};
