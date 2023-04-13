#include "pch_dx_11.h"
#include "LuminanacePass.h"

#include "Pipeline.h"

const UINT initSize = 729;//==3^5

LuminancePass::LuminancePass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
{
}

LuminancePass::~LuminancePass()
{
	greyScalePass->DeleteDestTextures();
	averagePass->DeleteDestTextures();
	averagePass0->DeleteDestTextures();
	SAFE_DELETE(greyScalePass);
	SAFE_DELETE(averagePass);
	SAFE_DELETE(averagePass0);
}

void LuminancePass::Init()
{
	greyScalePass = new GreyScalePass(dc, resources, pipeline, resizeNotice);
	greyScalePass->CreateDestTexture();
	averagePass = new AveragePass(dc, resources, pipeline, resizeNotice);
	//averagePass->CreateDestTexture();
	averagePass->CreateDestTexture();
	averagePass0 = new AveragePass(dc, resources, pipeline, resizeNotice);
	averagePass0->SetDestTexture(rtts[0]);
}

void LuminancePass::Set()
{
}

void LuminancePass::Execute()
{
	ClearRenderTargets();

	UINT size = initSize;

	//ȸ������ 3^5������� �̴´�.
	greyScalePass->ClearRenderTargets();
	greyScalePass->SetSize(size, size);
	greyScalePass->SetSourceTexture(sourceTextures[0]);
	greyScalePass->Execute();

	//3^4 ������� ����� ���ش�.
	size /= 3;
	averagePass->ClearRenderTargets();
	averagePass->SetSize(size, size);
	averagePass->SetSourceTexture(greyScalePass->GetDestTexture());
	averagePass->Execute();

	//3^3 ������� ����� ���ش�.
	//���� UAV ������� �𸣱⶧���� PASS�� 2���� ��������
	size /= 3;
	averagePass0->ClearRenderTargets();
	averagePass0->SetSize(size, size);
	averagePass0->SetSourceTexture(averagePass->GetDestTexture());
	averagePass0->Execute();

	//3^2 ������� ����� ���ش�.
	size /= 3;
	averagePass->ClearRenderTargets();
	averagePass->SetSize(size, size);
	averagePass->SetSourceTexture(averagePass0->GetDestTexture());
	averagePass->Execute();

	//3^1 ������� ����� ���ش�.
	//���� UAV ������� �𸣱⶧���� PASS�� 2���� ��������
	size /= 3;
	averagePass0->ClearRenderTargets();
	averagePass0->SetSize(size, size);
	averagePass0->SetSourceTexture(averagePass->GetDestTexture());
	averagePass0->Execute();

	//3^0 ������� ����� ���ش�.
	size /= 3;
	averagePass->ClearRenderTargets();
	averagePass->SetSize(size, size);
	averagePass->SetSourceTexture(averagePass0->GetDestTexture());
	averagePass->Execute();

	//�ϳ��� �߰�
	size /= 3;
	averagePass0->ClearRenderTargets();
	averagePass0->SetSize(size, size);
	averagePass0->SetSourceTexture(averagePass->GetDestTexture());
	averagePass0->Execute();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
}

void LuminancePass::CreateShader()
{
}
