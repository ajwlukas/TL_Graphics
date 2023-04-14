#pragma once

/// <summary>
/// ��ó�� bool ���� ���� �� ����ü, ���� �����ؼ� ���� �� ��
/// RenderSystem���� GetControlPanel �̿��� ��
/// 
/// ���� ���� : PostProcessor.h
/// </summary>

namespace TL_Graphics
{
	struct ControlPanel
	{
		bool doGrid = true;

		bool doColorGrading = false;

		bool doLightAdaption = false;
		float middleGrey = 0.5f;

		bool doToneMapping = false;
		float maxWhite = 7.0f;

		bool doBloom = false;
	};
}