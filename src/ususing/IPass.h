#pragma once

#include "RenderTargetTexture.h"
#include "QuadMesh.h"

class IPass
{
public:
	IPass();
	~IPass();

	virtual void OnResize();

	//����Ʈ ���μ����� srv, ���۵� ������ �ؽ��� or ���۵� ������ �ؽ��� ��ó�� �� �ؽ���, ȭ�鿡 �ش��ϴ� SRV
	//void SetMainSRV(Resource<ID3D11ShaderResourceView>* srv);

	virtual void ClearRenderTarget();

	//���� �۾��� �̷������ ��
	virtual void Draw() = 0;

	//����Ʈ ���μ��� �� srv
	virtual Resource<ID3D11ShaderResourceView>* GetSRV() { return &rtt.srv; }

	virtual RenderTargetTexutre* GetRTT() { return &rtt; }


protected:
	Transform transform;//������ ����� �� �ƴϸ� �ǹ� ����

	//Resource<ID3D11ShaderResourceView>* mainSRV;

	RenderTargetTexutre rtt;

	QuadMesh mesh;

	ID3D11PixelShader* pixelShader;
};