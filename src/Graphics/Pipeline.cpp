#include "pch_dx_11.h"
#include "Pipeline.h"

Pipeline::Pipeline(ID3D11DeviceContext* dc, IDXGISwapChain* swapChain, OnResizeNotice* resizeNotice, Resources* resources)
	:dc(dc), currentMaterial(nullptr), currentMesh(nullptr)
	,swapChain(swapChain)
	,resources(resources)
{
	resizeNotice->AddObserver(this);
	Init(resizeNotice->GetWidth(), resizeNotice->GetHeight());
}

Pipeline::~Pipeline()
{
}

void Pipeline::Init(UINT width, UINT height)
{
	CreateDefaultStates();

	SetCurrentRasterState(defaultRasterState);
	SetCurrentDepthStencilState(defaultDepthStencilState);
	SetCurrentBlendState(defaultBlendState);
	SetCurrentSamplerState(defualtSamplerState);

	OnResize(width, height);
}

void Pipeline::OnResize(uint32_t width, uint32_t height)
{
	ResizeSwapChainRtv(width, height);
	ResizeDepthStencilView(width, height);
	ResizeViewPort(width, height);

	SetRenderTarget_SwapChain();
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
	dc->ClearRenderTargetView(swapChainRtv, color);

	dc->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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

void Pipeline::SetConstantBuffer(ConstantBuffer* constantBuffer)
{
	if (constantBuffer->type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		dc->VSSetConstantBuffers(constantBuffer->slot, 1, constantBuffer->buffer);

		currentConstantBuffersVS[constantBuffer->slot] = constantBuffer;
	}

	else if (constantBuffer->type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		dc->PSSetConstantBuffers(constantBuffer->slot, 1, constantBuffer->buffer);

		currentConstantBuffersPS[constantBuffer->slot] = constantBuffer;
	}
}

void Pipeline::SetShaderResource(ShaderResource* shaderResource)
{
	if (shaderResource->type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		dc->VSSetShaderResources(shaderResource->slot, 1, shaderResource->srv);

		currentShaderResourceVS[shaderResource->slot] = shaderResource;
	}

	else if (shaderResource->type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		dc->PSSetShaderResources(shaderResource->slot, 1, shaderResource->srv);

		currentShaderResourcePS[shaderResource->slot] = shaderResource;
	}
}

void Pipeline::SetMaterial(Material* material)
{
	dc->PSSetShader(material->pixelShader, 0, 0);

	currentMaterial = material;
}

void Pipeline::SetRenderTarget(Resource<ID3D11RenderTargetView> rtv)
{
	dc->OMSetRenderTargets(1, rtv, depthStencilView);
}

void Pipeline::SetRenderTarget_SwapChain()
{
	dc->OMSetRenderTargets(1, swapChainRtv, depthStencilView);
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
}

void Pipeline::CreateDefaultStates()
{
	resources->rasterStates->GetDefault(defaultRasterState);
	resources->depthStencilStates->GetDefault(defaultDepthStencilState);
	resources->blendStates->GetDefault(defaultBlendState);
	resources->samplerStates->GetDefault(defualtSamplerState);
}

void Pipeline::ResizeSwapChainRtv(UINT width, UINT height)
{
	HRESULT hr = S_OK;

	swapChainRtv.Return();

	ID3D11Texture2D* backBuffer = nullptr;
	swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	resources->rtvs->CreateDefault(swapChainRtv, backBuffer);

	backBuffer->Release();
	backBuffer = nullptr;

	assert(SUCCEEDED(hr));
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
