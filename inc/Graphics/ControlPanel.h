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

		bool doDownSample = false;

		bool doGaussianBlur = false;

		bool doColorGrading = false;
	};
}