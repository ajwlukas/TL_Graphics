﻿#include "pch_dx_11.h"

#include "DX11Renderer.h"


//DX11Renderer* DX11Renderer::instance = nullptr;

DX11Renderer::DX11Renderer()
    :device(nullptr), dc(nullptr), hWnd(0), windowInfo{}, swapChain(nullptr), rtv{}, depthStencilBuffer{},
    depthStencilView{}, rasterState{}, width(0), height(0)
{
    onResizeNotice.AddObserver(this);
}

DX11Renderer::~DX11Renderer()
{
    depthStencilState.Return();
    noDepthStencilState.Return();

    SAFE_DELETE(resources);
    SAFE_RELEASE(device);
    SAFE_RELEASE(dc);
    SAFE_RELEASE(swapChain);
}

HRESULT DX11Renderer::Init()
{
    hWnd = GetActiveWindow();
    GetWindowInfo(hWnd, &windowInfo);
    width = windowInfo.rcClient.right - windowInfo.rcClient.left;
    height = windowInfo.rcClient.bottom - windowInfo.rcClient.top;

    OnResize(width, height);

    HRESULT hr = S_OK;

    hr = CreateDeviceAndSwapChain();
    if (hr != S_OK) return hr;

    resources = new Resources(device);
    pipeline = new Pipeline(dc);

    hr = CreateRtv();
    if (hr != S_OK) return hr;

    hr = CreateAndSetDepthStencilView();
    if (hr != S_OK) return hr;

    hr = CreateAndSetRasterizerState();
    if (hr != S_OK) return hr;

    hr = CreateBlendState();
    if (hr != S_OK) return hr;

    SetViewPort();


    return hr;
}

HRESULT DX11Renderer::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;//창모드 변경

    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = S_OK;
    D3D_FEATURE_LEVEL FeatureLevel;

    if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        &FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        &FeatureLevel,
        &dc)))
    {
        return hr;
    }

    return hr;
}

HRESULT DX11Renderer::CreateRtv()
{
    HRESULT hr = S_OK;

    rtv.Return();

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
    {
        return hr;
    }

    resources->rtvs->CreateDefault(rtv, backBuffer);

    backBuffer->Release();
    backBuffer = nullptr;

    return hr;
}

HRESULT DX11Renderer::CreateAndSetDepthStencilView()
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

    resources->depthStencilStates->GetDefault(depthStencilState);

    D3D11_DEPTH_STENCIL_DESC desc = {};

    desc.DepthEnable = false;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc = D3D11_COMPARISON_LESS;

    desc.StencilEnable = true;
    desc.StencilReadMask = 0xFF;
    desc.StencilWriteMask = 0xFF;

    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    resources->depthStencilStates->Get(noDepthStencilState,desc);

    dc->OMSetDepthStencilState(noDepthStencilState, 1);

    resources->depthStencilViews->CreateDefault(depthStencilView, depthStencilBuffer);

    dc->OMSetRenderTargets(1, rtv, depthStencilView);

    return hr;
}

HRESULT DX11Renderer::CreateAndSetRasterizerState()
{
    HRESULT hr = S_OK;

    resources->rasterStates->GetDefault(rasterState);

    dc->RSSetState(rasterState);

    return hr;
}

HRESULT DX11Renderer::CreateBlendState()
{
    HRESULT hr = S_OK;

    resources->blendStates->GetDefault(blendState);

    dc->OMSetBlendState(blendState, NULL, 0xFF);

    return hr;
}

void DX11Renderer::SetViewPort()
{
    D3D11_VIEWPORT viewPort = {};
    viewPort.Width = (float)width;
    viewPort.Height = (float)height;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;

    dc->RSSetViewports(1, &viewPort);
}


void DX11Renderer::OnResize(uint32_t _width, uint32_t _height)
{
    if (this == nullptr) return;

    GetWindowInfo(hWnd, &windowInfo);
        width = _width;
        height = _height;

        if (width == 0 && height == 0) return;

    if (dc != nullptr)
    {
        CreateRtv();

        CreateAndSetDepthStencilView();

        SetViewPort();
    }
}

void DX11Renderer::Clear()
{
    float color[4] = { 0.0f, 0.7f, 1.0f, 1.0f };
    float transparent[4] = { 0.0f,0.0f,0.0f,0.0f };//todo: 구차나서 일단 이렇게 해놓음
    float white[4] = { 1.0f,1.0f,1.0f,1.0f };//todo: 구차나서 일단 이렇게 해놓음

    dc->ClearRenderTargetView(rtv, color);

    dc->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DX11Renderer::Present()
{
    swapChain->Present(0, 0);
}

Mesh* DX11Renderer::CreateMesh(TL_Graphics::VertexSet& vertexSet, UINT indexData[], UINT indexCount, wstring fileName)
{
    return new Mesh(dc, resources, pipeline, vertexSet, indexData, indexCount, fileName);
}

Material* DX11Renderer::CreateMaterial(std::wstring fileName, const TL_Graphics::MaterialDesc& desc)
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    return new Material(dc, resources, pipeline, fileName, samplerDesc, desc);
}

ConstantBuffer* DX11Renderer::CreateConstantBuffer(UINT slot,TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize)
{
    return new ConstantBuffer(dc, resources, pipeline, slot, type, data, dataSize);
}

Camera* DX11Renderer::CreateCamera()
{
    return new Camera(dc, resources, pipeline, &onResizeNotice, 80.0f, width, height, 1.0f, 2000.0f);
}

void DX11Renderer::UpdateWindowSize(UINT width, UINT height)
{
    onResizeNotice.Update(width, height);
}

void DX11Renderer::Draw()
{
    pipeline->Draw();
}
