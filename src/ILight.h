#pragma once

#include "Export.h"

#include "Math\TL_Math.h"

namespace TL_Graphics
{
	 __declspec(align(16)) struct  DirectionalLight
	{
		TL_Math::Vector3 direction;
	};
	__declspec(align(16)) struct  PointLight
	{
		TL_Math::Vector3 position;
		float range;

		TL_Math::Vector3 attenuation;
	};
	__declspec(align(16)) struct  SpotLight
	{
		TL_Math::Vector3 position;
		float range;

		TL_Math::Vector3 direction;
		float spot;

		TL_Math::Vector3 attenuation;
	};


}