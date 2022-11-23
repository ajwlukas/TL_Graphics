#include "pch_dx_11.h"
#include "Pipeline.h"

Pipeline::Pipeline(ID3D11DeviceContext* dc, IDXGISwapChain* swapChain, OnResizeNotice* resizeNotice, Resources* resources)
	:dc(dc), currentMaterial(nullptr), currentMesh(nullptr)
	,swapChain(swapChain)
	,resources(resources)
{
	resizeNotice->AddObserver(this);

	swapChainRtv = new SwapChainRenderTarget(dc, resources, this, resizeNotice);

	Init(resizeNotice->GetWidth(), resizeNotice->GetHeight());

}

Pipeline::~Pipeline()
{
	SAFE_DELETE(swapChainRtv);
}

void Pipeline::Init(UINT width, UINT height)
{
	CreateDefaultStates();

	SetCurrentRasterState(defaultRasterState);
	SetCurrentDepthStencilState(defaultDepthStencilState);
	SetCurrentBlendState(defaultBlendState);
	SetCurrentSamplerState(defualtSamplerState);

	OnResize(width, height);

	swapChainRtv->OnResize(width, height);
}

void Pipeline::OnResize(uint32_t width, uint32_t height)
{
	ResizeDepthStencilView(width, height);
	ResizeViewPort(width, height);

	SetViewPort();
}

void Pipeline::SetMesh(Mesh* mesh)
{
	dc->IASetInputLayout(mesh->inputLayout);
	dc->IASetPrimitiveTopology(mesh->topology);
	dc->IASetVertexBuffers(0, 1, mesh->vertexBuffer, &mesh->vertexDataSize, &mesh->offset);
	dc->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->VSSetShader(mesh->shader, 0, 0);

	currentMesh = mesh;
}

void Pipeline::Clear(float color[4])
{
	dc->ClearRenderTargetView(swapChainRtv->rtv, color);

	dc->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Pipeline::ClearRenderTarget(RenderTarget* renderTarget, TL_Math::Vector4 color)
{
	float rgba[4] = { color.x,color.y,color.z,color.w };
	dc->ClearRenderTargetView(renderTarget->rtv, rgba);

}

void Pipeline::SetCurrentRasterState(Resource<ID3D11RasterizerState> state)
{
	dc->RSSetState(state);
	currentRasterState = state;
}

void Pipeline::SetViewPort()
{
	dc->RSSetViewports(1, &viewPort);
}

void Pipeline::SetConstantBuffer(ConstantBuffer* constantBuffer, TL_Graphics::E_SHADER_TYPE type,
	UINT slot)
{
	if (type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		dc->VSSetConstantBuffers(slot, 1, constantBuffer->buffer);

		currentConstantBuffersVS[slot] = constantBuffer;
	}

	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		dc->PSSetConstantBuffers(slot, 1, constantBuffer->buffer);

		currentConstantBuffersPS[slot] = constantBuffer;
	}
}

void Pipeline::SetShaderResource(ShaderResource* shaderResource, TL_Graphics::E_SHADER_TYPE type,
	UINT slot)
{
	if (type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		dc->VSSetShaderResources(slot, 1, shaderResource->srv);

		currentShaderResourceVS[slot] = shaderResource;
	}

	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		dc->PSSetShaderResources(slot, 1, shaderResource->srv);

		currentShaderResourcePS[slot] = shaderResource;
	}
}

void Pipeline::SetMaterial(Material* material, UINT albdeoMapSlot, UINT metallicMapSlot, UINT roughnessMapSlot)
{
	dc->PSSetShader(material->pixelShader, 0, 0);

	currentMaterial = material;
}

void Pipeline::SetShader(Shader* shader)
{
	if (shader->type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		dc->VSSetShader(*shader,0,0);
		currentVSShader = shader;
	}

	else if (shader->type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		dc->PSSetShader(*shader, 0, 0);
		currentPSShader = shader;
	}

}

void Pipeline::SetRenderTarget(RenderTarget* rtv, UINT slot)
{
	renderTargets[slot] = rtv->rtv;

	dc->OMSetRenderTargets(MAX_RENDERTARGET, renderTargets, depthStencilView);

	currentRenderTarget[slot] = rtv;
}

void Pipeline::SetSwapChainRenderTargetView(UINT slot)
{
	swapChainRtv->Set(slot);
}

void Pipeline::UnSetAllRenderTargets()
{
	for (UINT i = 0; i < MAX_RENDERTARGET; i++)
		UnSetRenderTarget(i);
}

void Pipeline::UnSetRenderTarget(UINT slot)
{
	renderTargets[slot] = nullptr;

	dc->OMSetRenderTargets(MAX_RENDERTARGET, renderTargets, depthStencilView);

	currentRenderTarget[slot] = nullptr;
}

void Pipeline::SetCurrentBlendState(Resource<ID3D11BlendState> state)
{
	dc->OMSetBlendState(state, NULL, 0xFF);
	currentBlendState = state;
}

void Pipeline::SetCurrentSamplerState(Resource<ID3D11SamplerState> state)
{
	dc->PSSetSamplers(0, 1, state);
	defualtSamplerState = state;
}


void Pipeline::SetCurrentDepthStencilState(Resource<ID3D11DepthStencilState> state)
{
	dc->OMSetDepthStencilState(state, 1);
	currentDepthStencilState = state;
}

void Pipeline::Draw()
{
	dc->DrawIndexed(currentMesh->GetIndexCount(), 0, 0);

	for (auto reserve : reservations)
		reserve();
	reservations.clear();
}

void Pipeline::DrawInstanced(UINT numInstance)
{
	dc->DrawIndexedInstanced(currentMesh->GetIndexCount(), numInstance, 0, 0, 0);
}

void Pipeline::CreateDefaultStates()
{
	resources->rasterStates->GetDefault(solidRasterState);
	resources->rasterStates->GetWireMode(wireFrameRasterState);

	defaultRasterState = solidRasterState;
	//defaultRasterState = wireFrameRasterState;



	resources->depthStencilStates->GetDefault(defaultDepthStencilState);
	resources->blendStates->GetDefault(defaultBlendState);
	resources->samplerStates->GetDefault(defualtSamplerState);
}

void Pipeline::ResizeDepthStencilView(UINT width, UINT height)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	resources->texture2Ds->Create(depthStencilBuffer, depthBufferDesc);

	resources->depthStencilViews->CreateDefault(depthStencilView, depthStencilBuffer);

	assert(SUCCEEDED(hr));
}

void Pipeline::ResizeViewPort(UINT width, UINT height)
{
	viewPort.Width = (float)width;
	viewPort.Height = (float)height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
}

void Pipeline::DrawWireOnce()
{
	SetCurrentRasterState(wireFrameRasterState);

	reservations.emplace_back(
		[&]() 
		{
			SetCurrentRasterState(defaultRasterState);
		}
	);
}

void Pipeline::DrawSolidOnce()
{

	SetCurrentRasterState(solidRasterState);

	reservations.emplace_back(
		[&]()
		{
			SetCurrentRasterState(defaultRasterState);
		}
	);
}

void Pipeline::SetSolidModeAsDefualt()
{
	defaultRasterState = solidRasterState;
	SetCurrentRasterState(defaultRasterState);
}

void Pipeline::SetWireModeAsDefualt()
{
	defaultRasterState = wireFrameRasterState;
	SetCurrentRasterState(defaultRasterState);
}
