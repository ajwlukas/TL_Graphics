#pragma once

/// <summary>
/// 후처리 bool 변수 관리 할 구조체, 직접 선언해서 쓰지 말 것
/// RenderSystem에서 GetControlPanel 이용할 것
/// 
/// 연관 파일 : PostProcessor.h
/// </summary>

namespace TL_Graphics
{
	struct ControlPanel
	{
		bool doGrid = true;

		class ITexture* cubeMap = nullptr;
		class ITexture* irradianceMap = nullptr;
		class ITexture* prefilteredEnvMap = nullptr;
		class ITexture* iblBRDF = nullptr;


		bool doColorGrading = false;

		class ITexture* colorGradingLUT = nullptr;

		bool doLightAdaption = false;
		float middleGrey = 0.5f;

		bool doToneMapping = false;
		float maxWhite = 7.0f;

		bool doBloom = false;
	};
}