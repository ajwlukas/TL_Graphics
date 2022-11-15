#include "pch_dx_11.h"
#include "RenderTargetTexture.h"

RenderTargetTexture::RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT width, UINT height)
	:ShaderResource(dc, resources, pipeline)
{
    OnResize(width, height);
}

RenderTargetTexture::~RenderTargetTexture()
{
}

void RenderTargetTexture::SetRT(UINT slot)
{
}

void RenderTargetTexture::SetT(TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
    ShaderResource::Set(type, slot);
}

void RenderTargetTexture::OnResize(uint32_t width, uint32_t height)
{
    if (!isBasedWindowSize) return;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    resources->texture2Ds->Create(texture, desc);

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = desc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;

    resources->rtvs->Create(rtv, rtvDesc, texture);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    resources->srvs->Create(srv, srvDesc, texture);
}
