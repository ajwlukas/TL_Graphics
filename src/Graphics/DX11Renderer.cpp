#include "pch_dx_11.h"

#include "DX11Renderer.h"

DX11Renderer::DX11Renderer()
    :device(nullptr), dc(nullptr), hWnd(0)
    
    , swapChain(nullptr)

{
}

DX11Renderer::~DX11Renderer()
{
    SAFE_DELETE(pipeline);
    SAFE_DELETE(resources);
    SAFE_RELEASE(dc);
    SAFE_RELEASE(device);
    SAFE_RELEASE(swapChain);
}

HRESULT DX11Renderer::Init()
{
    hWnd = GetActiveWindow();
   

    HRESULT hr = S_OK;

    hr = CreateDeviceAndSwapChain();
    if (hr != S_OK) return hr;

    resources = new Resources(device);
    pipeline = new Pipeline(dc, swapChain, &onResizeNotice, resources);


    return hr;
}

HRESULT DX11Renderer::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = onResizeNotice.GetWidth();
    swapChainDesc.BufferDesc.Height = onResizeNotice.GetHeight();
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


void DX11Renderer::Clear()
{
    float color[4] = { 0.0f, 0.7f, 1.0f, 1.0f };
    pipeline->Clear(color);
}

void DX11Renderer::Present()
{
    swapChain->Present(0, 0);
}

Mesh* DX11Renderer::CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, wstring fileName)
{
    return new Mesh(resources, pipeline, vertexSet, indexData, indexCount, fileName);
}

Material* DX11Renderer::CreateMaterial(std::wstring fileName, const TL_Graphics::MaterialDesc& desc)
{
    return new Material(resources, pipeline, fileName, desc);
}

ConstantBuffer* DX11Renderer::CreateConstantBuffer(UINT slot,TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize)
{
    return new ConstantBuffer(dc, resources, pipeline, slot, type, data, dataSize);
}

Camera* DX11Renderer::CreateCamera()
{
    return new Camera(dc, resources, pipeline, &onResizeNotice, 80.0f, 1.0f, 2000.0f);
}

Texture* DX11Renderer::CreateTexture(UINT slot, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName)
{
    return new Texture(dc, resources, pipeline, slot, type, fileName);
}

void DX11Renderer::UpdateWindowSize(UINT width, UINT height)
{
    onResizeNotice.Update(width, height);
}

void DX11Renderer::Draw()
{
    pipeline->Draw();
}
