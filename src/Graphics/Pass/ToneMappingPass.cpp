#include "pch_dx_11.h"
#include "ToneMappingPass.h"

#include "Pipeline.h"

ToneMappingPass::ToneMappingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:IRenderPass(dc, resources, pipeline, 1, 1)
{
	CreateShader();

	figureBuffer = new ConstantBuffer(dc, resources, pipeline, &m_Figure, sizeof(m_Figure));
	figureBuffer->SetDebugName("MaxWhite");
}

ToneMappingPass::~ToneMappingPass()
{
	SAFE_DELETE(shaderPS);
	SAFE_DELETE(figureBuffer);
}

void ToneMappingPass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();

	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);

	figureBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);
}

void ToneMappingPass::Execute()
{
	Set();
	auto oldViewport = pipeline->SetViewPort(&viewPort);

	pipeline->Draw();

	pipeline->UnSetAllRenderTargets();
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}

void ToneMappingPass::CreateRenderTargets(OnResizeNotice* resizeNotice)
{
}

void ToneMappingPass::DeleteRenderTargets()
{
}

void ToneMappingPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_ToneMapping.hlsl");
}
