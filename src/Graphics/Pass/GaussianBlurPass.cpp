#include "pch_dx_11.h"
#include "GaussianBlurPass.h"

#include "Pipeline.h"

GaussianBlurPass::GaussianBlurPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, TL_Math::Vector2 direction)
	:IRenderPass(dc, resources, pipeline)
	
{
	dir_info.dir = direction;


	dirInfoBuffer = new ConstantBuffer(dc, resources, pipeline, &dir_info, sizeof(Dir_Info));

	CreateRenderTarget(resizeNotice);
	CreateShader();
}

GaussianBlurPass::~GaussianBlurPass()
{
	SAFE_DELETE(dirInfoBuffer);

	SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void GaussianBlurPass::Set()
{
	rtt->SetRT(0);

	shaderPS->Set();

	dirInfoBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);
}

void GaussianBlurPass::Execute()
{
	ClearRenderTargets();
	Set();


	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	rtt->SetT(TL_Graphics::E_SHADER_TYPE::PS, interPassSlot);
}

void GaussianBlurPass::ClearRenderTargets()
{
	rtt->Clear();
}

void GaussianBlurPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtt = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "GaussainBlur");
}

void GaussianBlurPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/GaussianBlur.hlsl", "GaussainBlur");

}
