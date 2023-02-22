#pragma once
#include <unordered_map>
#include <string>
#include <d3d11.h>

#include "Resource.h"
#include "Utility.h"
/// <summary>
/// �������̸� �����ڳ�, Init()���� ������ �ѹ��� ȣ���ؼ� �޾Ƽ� ����ϱ� ����(�˻� �ѹ��� �ϵ���)
/// Resource<ResourceType>���� ���, ��� �� Return() ���� ��
/// written by Ssreppa 22.06.28
/// 
/// Get()�Լ��� Create()�Լ� ����, Get()�� ���� �ɼ��̸� �ִ��� ��,
/// Create()�� ���� �ɼ��̾ ���� ����� ��
/// �� �̻� Return()���� �ʿ� ����, �� �����ϰ� ����� ���� ��, �Ҹ��� ���� ���ص� ��
/// written by Ssreppa 22.07.01
/// 
/// 
///template <typename T>
///class Resource
///{
///public:
///	operator T* () { return resource; }
///	operator T** () { return &resource; }
///
///	T* resource;
///
///	std::function<void()> Return;
///};
/// </summary>

#ifdef _DEBUG
#define SHADERFLAG D3DCOMPILE_DEBUG | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR |\
D3DCOMPILE_PARTIAL_PRECISION | D3DCOMPILE_OPTIMIZATION_LEVEL0

#else
#define SHADERFLAG D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR |\
D3DCOMPILE_PARTIAL_PRECISION | D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif

class DX11Renderer;

class InputLayoutResources;

class VertexShaderResources;
class PixelShaderResources;

class SRVResources;

class SamplerStateResources;
class RasterizerStateResources;
class DepthStencilStateResources;
class BlendStateResources;

class BufferResources;
class Texture2DResources;
class RenderTargetViewResources;
class DepthStencilViewResources;

//todo : refCount �ؼ� �Ⱦ��� ���ҽ� �ڵ� ���� �ϵ��� ���� ��

class Resources//��� �ڿ� �Ŵ����� �����ִ� Ŭ����
{
public:
	Resources(ID3D11Device* device);
	~Resources();
public:
	ID3D11Device* device;

	VertexShaderResources* vertexShaders;
	InputLayoutResources* inputLayouts;
	PixelShaderResources* pixelShaders;
	SRVResources* srvs;
	SamplerStateResources* samplerStates;
	RasterizerStateResources* rasterStates;
	DepthStencilStateResources* depthStencilStates;
	BlendStateResources* blendStates;
	Texture2DResources* texture2Ds;
	RenderTargetViewResources* rtvs;
	DepthStencilViewResources* depthStencilViews;
	BufferResources* buffers;
};

class VertexShaderResources
{
public:
	void Get(Resource< ID3D11VertexShader>& dest, std::wstring shaderFileName);

	ID3DBlob* GetBlob(std::wstring shaderFileName);

private:
	friend class Resources;
	Resources* resources;
	VertexShaderResources(Resources* resources) : resources(resources) {}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11VertexShader* data;
		UINT refCount;
	};

	std::unordered_map<std::wstring, Data> vertexShaders;
	std::unordered_map<std::wstring, ID3DBlob*> vertexShaderBlobs;
	ID3DBlob* error = nullptr;
};

class InputLayoutResources
{
public:
	void Get(Resource< ID3D11InputLayout>& dest, D3D11_INPUT_ELEMENT_DESC* desc, UINT descSize, std::wstring vertexShaderFileName);

private:
	friend class Resources;
	Resources* resources;
	InputLayoutResources(Resources* resources) : resources(resources) {}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11InputLayout* data;
		UINT refCount;
	};
	std::unordered_map<std::string, Data> inputLayouts;
};

class PixelShaderResources
{
public:
	void Get(Resource< ID3D11PixelShader>& dest, std::wstring shaderFileName);
	ID3DBlob* GetBlob(std::wstring shaderFileName);

private:
	friend class Resources;
	Resources* resources;
	PixelShaderResources(Resources* resources) : resources(resources) {}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11PixelShader* data;
		UINT refCount;
	};

	std::unordered_map<std::wstring, Data> pixelShaders;
	std::unordered_map<std::wstring, ID3DBlob*> pixelShaderBlobs;
	ID3DBlob* error;
};

class SRVResources
{
public:
	void Create(Resource<ID3D11ShaderResourceView>& dest, D3D11_SHADER_RESOURCE_VIEW_DESC desc, ID3D11Resource* buffer);
	void GetFromFile(Resource< ID3D11ShaderResourceView>& dest, std::wstring fileName);

private:
	friend class Resources;
	Resources* resources;
	SRVResources(Resources* resources) : resources(resources) {}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11ShaderResourceView* data;
		UINT refCount;
	};
	std::unordered_map<ID3D11ShaderResourceView*, Data> srvs;
	std::unordered_map<std::wstring, Data> srvsFromTexture;
};

/// States for DX
///todo : SamplerState�� AddressMode ���� ���� ���� ����.
class SamplerStateResources
{
public:
	void Get(Resource< ID3D11SamplerState>& dest, D3D11_SAMPLER_DESC desc);

	void GetWrap(Resource< ID3D11SamplerState>& dest) { Get(dest, wrapDesc); }
	void GetMirror(Resource< ID3D11SamplerState>& dest) { Get(dest, mirrorDesc); }
	void GetClamp(Resource< ID3D11SamplerState>& dest) { Get(dest, clampDesc); }
	void GetBorder(Resource< ID3D11SamplerState>& dest) { Get(dest, borderDesc); }

	void SetWrap(D3D11_SAMPLER_DESC desc);
	void SetMirror(D3D11_SAMPLER_DESC desc);
	void SetClamp(D3D11_SAMPLER_DESC desc);
	void SetBorder(D3D11_SAMPLER_DESC desc);
private:
	friend class Resources;
	Resources* resources;
	SamplerStateResources(Resources* resources) : resources(resources)
	{
		D3D11_SAMPLER_DESC desc = {};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MinLOD = 0;
		desc.MaxLOD = 10;
		desc.MipLODBias = 0;

		SetWrap(desc);

		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

		SetMirror(desc);

		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;



		SetClamp(desc);

		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;


		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 0.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;

		SetBorder(desc);
	}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11SamplerState* data;
		UINT refCount;
	};
	D3D11_SAMPLER_DESC wrapDesc;
	D3D11_SAMPLER_DESC mirrorDesc;
	D3D11_SAMPLER_DESC clampDesc;
	D3D11_SAMPLER_DESC borderDesc;
	std::unordered_map<std::string, Data> samplerStates;
};

class RasterizerStateResources
{
public:

	void Get(Resource< ID3D11RasterizerState>& dest, D3D11_RASTERIZER_DESC desc);

	void GetDefault(Resource< ID3D11RasterizerState>& dest) { Get(dest, defaultDesc); }
	void GetWireMode(Resource< ID3D11RasterizerState>& dest) { Get(dest, wireFrameDesc); }
	void SetDefault(D3D11_RASTERIZER_DESC desc);

private:
	friend class Resources;
	Resources* resources;
	RasterizerStateResources(Resources* resources) : resources(resources) {
		D3D11_RASTERIZER_DESC desc = {};
		desc.AntialiasedLineEnable = false;
		desc.CullMode = D3D11_CULL_BACK;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		desc.FrontCounterClockwise = true;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;

		SetDefault(desc);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		wireFrameDesc = desc;
	}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11RasterizerState* data;
		UINT refCount;
	};
	D3D11_RASTERIZER_DESC defaultDesc;
	D3D11_RASTERIZER_DESC wireFrameDesc;
	std::unordered_map<std::string, Data> rasterizerStates;
};

class DepthStencilStateResources
{
public:
	void Get(Resource< ID3D11DepthStencilState>& dest, D3D11_DEPTH_STENCIL_DESC desc);

	void GetDepthEnabled(Resource< ID3D11DepthStencilState>& dest) { Get(dest, depthEnabledDesc); }
	void SetDepthEnabled(D3D11_DEPTH_STENCIL_DESC desc);

	void GetDepthDisabled(Resource< ID3D11DepthStencilState>& dest) { Get(dest, depthDisabledDesc); }
	void SetDepthDisabled(D3D11_DEPTH_STENCIL_DESC desc);

private:
	friend class Resources;
	Resources* resources;
	DepthStencilStateResources(Resources* resources) : resources(resources) {
		D3D11_DEPTH_STENCIL_DESC desc = {};

		desc.DepthEnable = true;
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

		SetDepthEnabled(desc);

		desc.DepthEnable = false;

		SetDepthDisabled(desc);
	}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11DepthStencilState* data;
		UINT refCount;
	};
	D3D11_DEPTH_STENCIL_DESC depthEnabledDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledDesc;
	std::unordered_map<std::string, Data> depthStencilStates;
};

class BlendStateResources
{
public:
	void Get(Resource<ID3D11BlendState>& dest, D3D11_BLEND_DESC desc);

	void GetDefault(Resource<ID3D11BlendState>& dest) { Get(dest, defaultDesc); };
	void SetDefault(D3D11_BLEND_DESC desc);

private:
	friend class Resources;
	Resources* resources;
	BlendStateResources(Resources* resources) : resources(resources) {
		D3D11_BLEND_DESC desc = {};
		desc.RenderTarget[0].BlendEnable = TRUE;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		SetDefault(desc);

	}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11BlendState* data;
		UINT refCount;
	};
	D3D11_BLEND_DESC defaultDesc;
	std::unordered_map<std::string, Data> blendStates;
};

/////////////////////////////////////////////////////////////// �������� �ʴ� �ڿ���

class Texture2DResources
{
public:
	void Create(Resource<ID3D11Texture2D>& dest, D3D11_TEXTURE2D_DESC desc, const D3D11_SUBRESOURCE_DATA* pInitData = NULL);

private:
	friend class Resources;
	Resources* resources;
	Texture2DResources(Resources* resources) : resources(resources) {}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11Texture2D* data;
		UINT refCount;
	};
	std::unordered_map<ID3D11Texture2D*, Data> texture2Ds;// leak �ȳ��� �����ϴ� �뵵, ��ȿ���������� �ٸ� ����� ������ �ȳ�
};

class BufferResources
{
public:
	void Create(Resource<ID3D11Buffer>& dest, D3D11_BUFFER_DESC desc, const D3D11_SUBRESOURCE_DATA* pInitData = NULL);

private:
	friend class Resources;
	Resources* resources;
	BufferResources(Resources* resources) : resources(resources) {}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11Buffer* data;
		UINT refCount;
	};
	std::unordered_map<ID3D11Buffer*, Data> buffers;// leak �ȳ��� �����ϴ� �뵵, ��ȿ���������� �ٸ� ����� ������ �ȳ�
};

class RenderTargetViewResources
{
public:
	void Create(Resource<ID3D11RenderTargetView>& dest, D3D11_RENDER_TARGET_VIEW_DESC desc, ID3D11Resource* buffer);

	void CreateDefault(Resource<ID3D11RenderTargetView>& dest, ID3D11Resource* buffer) { Create(dest, defaultDesc, buffer); };
	void SetDefault(D3D11_RENDER_TARGET_VIEW_DESC desc) { defaultDesc = desc; };
private:
	friend class Resources;
	Resources* resources;
	RenderTargetViewResources(Resources* resources) : resources(resources) {
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		SetDefault(desc);
	}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11RenderTargetView* data;
		UINT refCount;
	};
	D3D11_RENDER_TARGET_VIEW_DESC defaultDesc;
	std::unordered_map<ID3D11RenderTargetView*, Data> rtvs;// leak �ȳ��� �����ϴ� �뵵
};


class DepthStencilViewResources
{
public:
	void Create(Resource<ID3D11DepthStencilView>& dest, D3D11_DEPTH_STENCIL_VIEW_DESC desc, ID3D11Resource* buffer);

	void CreateDefault(Resource<ID3D11DepthStencilView>& dest, ID3D11Resource* buffer) { Create(dest, defaultDesc, buffer); };
	void SetDefault(D3D11_DEPTH_STENCIL_VIEW_DESC desc) { defaultDesc = desc; };
private:
	friend class Resources;
	Resources* resources;
	DepthStencilViewResources(Resources* resources) : resources(resources) {
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};


		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		SetDefault(desc);
	}

	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11DepthStencilView* data;
		UINT refCount;
	};
	D3D11_DEPTH_STENCIL_VIEW_DESC defaultDesc;
	std::unordered_map<ID3D11DepthStencilView*, Data> depthStencilViews;// leak �ȳ��� �����ϴ� �뵵
};



