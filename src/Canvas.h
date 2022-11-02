#pragma once

#include "CanvasManager.h"

/// <summary>
/// ����Ʈ�� ���� ����, ����, 0.5 , 0.0 , 0.5, 0.5 �ϸ� ȭ�� ������ ��� 4���� 1 �����ϰ�,
///  uv ��ǥ�� ����
/// 
/// written by Ssreppa 22 06 18
/// </summary>
class Canvas : public Object//todo : ���� layer ���� �� �ְ� �߰� �� ��
{
public:
	Canvas(float posLeftRatio, float posTopRatio, float widthRatio, float heightRatio, UINT layer = 0);
	~Canvas();

	virtual void Update() override;
	virtual void Render() override;

	void ReSizeCanvas(float in_posLeftRatio, float in_posTopRatio, float in_widthRatio, float in_heightRatio);
	void SetSRV(Resource<ID3D11ShaderResourceView>* srvResource) { srv = srvResource; }

	Resource<ID3D11ShaderResourceView>* srv;

private:

	UINT layer ;

	float posLeftRatio, posTopRatio, widthRatio, heightRatio;
	void CreateCanvas();
	void ReCreateCanvas();

public:
	static CanvasManager manager;
};