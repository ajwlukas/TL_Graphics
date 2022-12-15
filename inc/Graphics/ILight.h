#pragma once

#include "Export.h"

#include "Math\TL_Math.h"

namespace TL_Graphics
{
	 __declspec(align(16)) struct DirectionalLight
	{
	public:
		const int type = 0;

	private:
		TL_Math::Vector3 position;

	public:
		float intensity;
		TL_Math::Vector3 direction;

	private:
		float range;
		TL_Math::Vector3 attenuation;

		float spot;
	public:
		TL_Math::Vector3 color;
	};
	__declspec(align(16)) struct PointLight
	{
	public:
		const int type = 1;
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
		const int type = 2;
		TL_Math::Vector3 position;

		float intensity;
		TL_Math::Vector3 direction;

		float range;
		TL_Math::Vector3 attenuation;

		float spot;
		TL_Math::Vector3 color;
	};


}