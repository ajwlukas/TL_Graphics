#pragma once

#include "ILight.h"
#include "TextureBuffer.h"

class Light
{
public:
	Light(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~Light();

	void BeginLightSet();

	void SetLight(const TL_Graphics::DirectionalLight* light);
	void SetLight(const TL_Graphics::PointLight* light);
	void SetLight(const TL_Graphics::SpotLight* light);

	void EndLightSet();

private:
	friend class Shadow;

	TextureBuffer* textureBuffer;

	const UINT maxLightNum;
	
	struct MetaData
	{
		float lightCount;//���̴����� ��꿡 �����ϴ� ����Ʈ�� ��, ���̴����� Ÿ�� ���� ������ float�� �����

		TL_Math::Vector3 pad;
	}metaData;

	//directionals dirs
	//static vector<TL_Math::Vector3> dirs;
};