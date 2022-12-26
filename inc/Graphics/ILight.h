#pragma once

#include "Export.h"

#include "Math\TL_Math.h"

namespace TL_Graphics
{
	 __declspec(align(16)) struct DirectionalLight
	{
	public:
		const float type = 0;//쉐이더에서 타입 문제 때문에 float로 사용함

	private:
		TL_Math::Vector3 position = {0,0,0};

	public:
		float intensity = 1.0f;
		TL_Math::Vector3 direction = { 1,0,0 };

	private:
		float range = 0;
		TL_Math::Vector3 attenuation = { 0,0,0 };

		float spot = 0;
	public:
		TL_Math::Vector3 color = { 1,0,1 };
	};
	__declspec(align(16)) struct PointLight
	{
	public:
		const float type = 1;//쉐이더에서 타입 문제 때문에 float로 사용함
		TL_Math::Vector3 position;

		float intensity;
	private:
		TL_Math::Vector3 direction;

	public:
		float range;
		TL_Math::Vector3 attenuation;

	private:
		float spot;
	public:
		TL_Math::Vector3 color;
	};
	__declspec(align(16)) struct SpotLight
	{
	public:
		const float type = 2;//쉐이더에서 타입 문제 때문에 float로 사용함
		TL_Math::Vector3 position;

		float intensity;
		TL_Math::Vector3 direction;

		float range;
		TL_Math::Vector3 attenuation;

		float spot;
		TL_Math::Vector3 color;
	};

}