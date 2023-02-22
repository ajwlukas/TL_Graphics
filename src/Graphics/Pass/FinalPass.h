#pragma once

///�׳� ����ü�ο� �׷��ִ�(Sampling) �н�,
///�н����� ��� ���������� ������ �ǰ� �� ���� �ְ�����
/// �ణ? �����ϰų� �ָ��� �κ��� ���� ���� ���� �� ���Ƽ� �ϴ�
/// �̷��� ��



#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class FinalPass : public IRenderPass
{
public:
	FinalPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~FinalPass();

	// IRenderPass��(��) ���� ��ӵ�
	virtual void Set() override;

	virtual void Execute() override;


private:

	Shader* shaderPS;
private:
	void CreateShader();
};