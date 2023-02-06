#include "pch_dx_11.h"
#include "Resources.h"

#include "Utility.h"



Resources::Resources(ID3D11Device* device)
	:device(device)
{
	vertexShaders = new VertexShaderResources(this);
	inputLayouts = new InputLayoutResources(this);
	pixelShaders = new PixelShaderResources(this);
	srvs = new SRVResources(this);
	samplerStates = new SamplerStateResources(this);
	rasterStates = new RasterizerStateResources(this);
	depthStencilStates = new DepthStencilStateResources(this);
	blendStates = new BlendStateResources(this);
	texture2Ds = new Texture2DResources(this);
	rtvs = new RenderTargetViewResources(this);
	depthStencilViews = new DepthStencilViewResources(this);
	buffers = new BufferResources(this);
}

Resources::~Resources()
{
	vertexShaders->Release();
	inputLayouts->Release();
	srvs->Release();
	pixelShaders->Release();
	samplerStates->Release();
	rasterStates->Release();
	depthStencilStates->Release();
	blendStates->Release();
	texture2Ds->Release();
	rtvs->Release();
	depthStencilViews->Release();
	buffers->Release();

	SAFE_DELETE(vertexShaders);
	SAFE_DELETE(inputLayouts);
	SAFE_DELETE(pixelShaders);
	SAFE_DELETE(srvs);
	SAFE_DELETE(samplerStates);
	SAFE_DELETE(rasterStates);
	SAFE_DELETE(depthStencilStates);
	SAFE_DELETE(blendStates);
	SAFE_DELETE(texture2Ds);
	SAFE_DELETE(rtvs);
	SAFE_DELETE(depthStencilViews);
	SAFE_DELETE(buffers);
}

void VertexShaderResources::Get(Resource< ID3D11VertexShader>& dest, std::wstring shaderFileName)
{
	dest.Return();

	if (vertexShaders.find(shaderFileName) == vertexShaders.end())//�ش��ϴ� ���̴��� ������
	{
		HRESULT hr;
		if (Utility::ExistFileW(shaderFileName))
		{
			hr = resources->device->CreateVertexShader(GetBlob(shaderFileName)->GetBufferPointer(), GetBlob(shaderFileName)->GetBufferSize(), NULL, &vertexShaders[shaderFileName].data);
		}
		else//cso�� ���� ����
		{
			string cso = Utility::GetPathNameWithoutExtension(Utility::ToString(shaderFileName)) + ".cso";

			std::vector<char> compiledShader = Utility::GetBinary(cso);

			hr = resources->device->CreateVertexShader(compiledShader.data(), compiledShader.size(), NULL, &vertexShaders[shaderFileName].data);
		}

		assert(SUCCEEDED(hr));
	}//����

	//������ �׳�, ������ ������ ��
	dest.resource = vertexShaders[shaderFileName].data;

	// ��� Ƚ�� �÷��ְ�
	dest.Copy = [&, shaderFileName]()
	{
		vertexShaders[shaderFileName].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, shaderFileName]()
	{
		vertexShaders[shaderFileName].refCount--;
		if (vertexShaders[shaderFileName].refCount == 0)
		{
			ID3D11VertexShader* del = vertexShaders[shaderFileName].data;
			vertexShaders.erase(shaderFileName);
			SAFE_RELEASE(del);
		}
	};

}

ID3DBlob* VertexShaderResources::GetBlob(wstring shaderFileName)
{
	if (vertexShaderBlobs.find(shaderFileName) == vertexShaderBlobs.end())//�ش��ϴ� ���̴��� ������
	{
		wstring path = shaderFileName;
		//D3D_COMPILE_STANDARD_FILE_INCLUDE == hlsl���� #include �� �� �ְ� ���ִ� ��
		HRESULT hr = D3DCompileFromFile(path.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
			SHADERFLAG, 0, &vertexShaderBlobs[shaderFileName], &error);//todo : Vertex.hlsl �ۼ� ����


		assert(SUCCEEDED(hr));


	}//����

	return vertexShaderBlobs[shaderFileName];
}

void VertexShaderResources::Release()
{
	for (auto shader : vertexShaderBlobs)
		shader.second->Release();
}


void InputLayoutResources::Get(Resource<ID3D11InputLayout>& dest, D3D11_INPUT_ELEMENT_DESC* desc, UINT descSize, wstring vertexShaderFileName)
{
	dest.Return();

	string key = {};
	for (UINT i = 0; i < descSize; i++)
		key += (string)desc[i].SemanticName;
	string vsName(vertexShaderFileName.begin(), vertexShaderFileName.end());
	key = key + vsName;

	if (inputLayouts.find(key) == inputLayouts.end())//�� ã����
	{
		HRESULT hr;
		if (Utility::ExistFileW(vertexShaderFileName))
		{
			hr = resources->device->CreateInputLayout(desc, descSize, resources->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferPointer(),
				resources->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferSize(), &inputLayouts[key].data);
		}
		else//cso�� ���� ����
		{
			string cso = Utility::GetPathNameWithoutExtension(Utility::ToString(vertexShaderFileName)) + ".cso";

			std::vector<char> compiledShader = Utility::GetBinary(cso);

			hr = resources->device->CreateInputLayout(desc, descSize, compiledShader.data(),
				compiledShader.size(), &inputLayouts[key].data);

		}

		assert(SUCCEEDED(hr));
	}//����

	//������ �׳�, ������ ������ ��
	dest.resource = inputLayouts[key].data;

	// ��� Ƚ�� �÷��ְ�
	dest.Copy = [&, key]()
	{
		inputLayouts[key].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, key]()
	{
		inputLayouts[key].refCount--;
		if (inputLayouts[key].refCount == 0)
		{
			ID3D11InputLayout* del = inputLayouts[key].data;
			inputLayouts.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void InputLayoutResources::Release()
{
	for (auto inputLayout : inputLayouts)
		SAFE_RELEASE(inputLayout.second.data);
}

void PixelShaderResources::Get(Resource< ID3D11PixelShader>& dest, wstring shaderFileName)
{
	dest.Return();

	if (pixelShaders.find(shaderFileName) == pixelShaders.end())//�ش��ϴ� ���̴��� ������
	{
		HRESULT hr;
		if (Utility::ExistFileW(shaderFileName))
		{
			hr = resources->device->CreatePixelShader(GetBlob(shaderFileName)->GetBufferPointer(), GetBlob(shaderFileName)->GetBufferSize(), NULL, &pixelShaders[shaderFileName].data);
		}
		else//cso�� ���� ����
		{
			string cso = Utility::GetPathNameWithoutExtension(Utility::ToString(shaderFileName)) + ".cso";

			std::vector<char> compiledShader = Utility::GetBinary(cso);

			hr = resources->device->CreatePixelShader(compiledShader.data(), compiledShader.size(), NULL, &pixelShaders[shaderFileName].data);
		}
		assert(SUCCEEDED(hr));
	}//����


	//������ �׳�, ������ ������ ��
	dest.resource = pixelShaders[shaderFileName] .data;

	// ��� Ƚ�� �÷��ְ�
	dest.Copy = [&, shaderFileName]()
	{
		pixelShaders[shaderFileName].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, shaderFileName]()
	{
		pixelShaders[shaderFileName].refCount--;
		if (pixelShaders[shaderFileName].refCount == 0)
		{
			ID3D11PixelShader* del = pixelShaders[shaderFileName].data;
			pixelShaders.erase(shaderFileName);
			SAFE_RELEASE(del);
		}
	};
}

ID3DBlob* PixelShaderResources::GetBlob(wstring shaderFileName)
{
	if (pixelShaderBlobs.find(shaderFileName) == pixelShaderBlobs.end())//�ش��ϴ� ���̴��� ������
	{
		wstring path = shaderFileName;
		HRESULT hr = D3DCompileFromFile(path.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
			SHADERFLAG, 0, &pixelShaderBlobs[shaderFileName], &error);//todo : Pixel.hlsl �ۼ� ����

		assert(SUCCEEDED(hr));
	}//����

	return pixelShaderBlobs[shaderFileName];
}

void PixelShaderResources::Release()
{
	for (auto shader : pixelShaderBlobs)
		shader.second->Release();
}

void SRVResources::Create(Resource<ID3D11ShaderResourceView>& dest, D3D11_SHADER_RESOURCE_VIEW_DESC desc, ID3D11Resource* buffer)
{
	dest.Return();

	HRESULT hr = resources->device->CreateShaderResourceView(buffer, &desc, dest);

	assert(SUCCEEDED(hr));

	//������ �׳�, ������ ������ ��
	srvs[dest].data = dest.resource;

	// ��� Ƚ�� �÷��ְ�
	dest.Copy = [&]()
	{
		srvs[dest].refCount++;
	};

	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&]()
	{
		srvs[dest].refCount--;
		if (srvs[dest].refCount == 0)
		{
			ID3D11ShaderResourceView* del = srvs[dest].data;
			if (srvs.find(del) != srvs.end())
				srvs.erase(del);
			SAFE_RELEASE(del);
		}
	};
}

void SRVResources::GetFromFile(Resource<ID3D11ShaderResourceView>& dest, wstring fileName)
{
	dest.Return();


	if (srvsFromTexture.find(fileName) == srvsFromTexture.end())//�ش��ϴ� srv�� ������
	{
		ID3D11Resource* res;

		wstring path = fileName;
		//todo : Ȯ���� üũ

		wstring extension = Utility::GetExtension(fileName);
		HRESULT hr = S_OK;
		if (extension == L"dds")
			hr = CreateDDSTextureFromFile(resources->device, path.c_str(), &res, &srvsFromTexture[fileName].data);
		else
			hr = CreateWICTextureFromFile(resources->device, path.c_str(), &res, &srvsFromTexture[fileName].data);


		res->Release();

		string fileNameWithNoPath = Utility::GetFileNameWithoutExtension(Utility::ToString(path));
		srvsFromTexture[fileName].data->SetPrivateData(WKPDID_D3DDebugObjectName, fileNameWithNoPath.length(), fileNameWithNoPath.c_str());

		assert(SUCCEEDED(hr));


	}//����

	//������ �׳�, ������ ������ ��
	dest.resource = srvsFromTexture[fileName].data;

	// ��� Ƚ�� �÷��ְ�
	dest.Copy = [&, fileName]()
	{
		srvsFromTexture[fileName].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, fileName]()
	{
		srvsFromTexture[fileName].refCount--;
		if (srvsFromTexture[fileName].refCount == 0)
		{
			ID3D11ShaderResourceView* del = srvsFromTexture[fileName].data;
			srvsFromTexture.erase(fileName);
			SAFE_RELEASE(del);
		}
	};
}

void SRVResources::Release()
{
	for (auto srv : srvsFromTexture)
		SAFE_RELEASE(srv.second.data);

	for (auto srv : srvs)
		SAFE_RELEASE(srv.second.data);
}

void SamplerStateResources::Get(Resource<ID3D11SamplerState>& dest, D3D11_SAMPLER_DESC desc)
{
	dest.Return();

	string key;
	char token = '_';
	key = to_string(desc.AddressU) + token + to_string(desc.AddressV) + token + to_string(desc.AddressW) + token
		+ to_string(desc.BorderColor[0]) + token + to_string(desc.BorderColor[1]) + token
		+ to_string(desc.BorderColor[2]) + token + to_string(desc.BorderColor[3]) + token
		+ to_string(desc.ComparisonFunc) + token + to_string(desc.Filter)
		+ token + to_string(desc.MaxAnisotropy) + token + to_string(desc.MaxLOD)
		+ token + to_string(desc.MinLOD) + token + to_string(desc.MipLODBias);

	if (samplerStates.find(key) == samplerStates.end())//�ش��ϴ� srv�� ������
	{
		HRESULT hr = resources->device->CreateSamplerState(&desc, &samplerStates[key].data);

		assert(SUCCEEDED(hr));
	}//����

	//������ �׳�, ������ ������ ��
	dest.resource = samplerStates[key].data;


	dest.Copy = [&, key]()
	{
		// ��� Ƚ�� �÷��ְ�
		samplerStates[key].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, key]()
	{
		samplerStates[key].refCount--;
		if (samplerStates[key].refCount == 0)
		{
			ID3D11SamplerState* del = samplerStates[key].data;
			samplerStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void SamplerStateResources::SetWrap(D3D11_SAMPLER_DESC desc)
{
	wrapDesc = desc;
}

void SamplerStateResources::SetMirror(D3D11_SAMPLER_DESC desc)
{
	mirrorDesc = desc;
}

void SamplerStateResources::SetClamp(D3D11_SAMPLER_DESC desc)
{
	clampDesc = desc;
}

void SamplerStateResources::SetBorder(D3D11_SAMPLER_DESC desc)
{
	borderDesc = desc;
}

void SamplerStateResources::Release()
{
	for (auto samplerState : samplerStates)
		samplerState.second.data->Release();
}

void RasterizerStateResources::Get(Resource<ID3D11RasterizerState>& dest, D3D11_RASTERIZER_DESC desc)
{
	dest.Return();

	string key = "";
	char token = '_';
	key = to_string(desc.FillMode) + token
		+ to_string(desc.CullMode) + token
		+ to_string(desc.FrontCounterClockwise) + token
		+ to_string(desc.DepthBias) + token
		+ to_string(desc.DepthBiasClamp) + token
		+ to_string(desc.SlopeScaledDepthBias) + token
		+ to_string(desc.DepthClipEnable) + token
		+ to_string(desc.ScissorEnable) + token
		+ to_string(desc.MultisampleEnable) + token
		+ to_string(desc.AntialiasedLineEnable);


	if (rasterizerStates.find(key) == rasterizerStates.end())//�ش��ϴ� srv�� ������
	{
		HRESULT hr = resources->device->CreateRasterizerState(&desc, &rasterizerStates[key].data);

		assert(SUCCEEDED(hr));
	}//����

	//������ �׳�, ������ ������ ��
	dest.resource = rasterizerStates[key].data;

	dest.Copy = [&, key]()
	{
		// ��� Ƚ�� �÷��ְ�
		rasterizerStates[key].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, key]()
	{
		rasterizerStates[key].refCount--;
		if (rasterizerStates[key].refCount == 0)
		{
			ID3D11RasterizerState* del = rasterizerStates[key].data;
			rasterizerStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void RasterizerStateResources::SetDefault(D3D11_RASTERIZER_DESC desc)
{
	defaultDesc = desc;
}

void RasterizerStateResources::Release()
{
	for (auto rasterizerState : rasterizerStates)
		rasterizerState.second.data->Release();
}

void DepthStencilStateResources::Get(Resource<ID3D11DepthStencilState>& dest, D3D11_DEPTH_STENCIL_DESC desc)
{
	dest.Return();

	string key = "";
	char token = '_';
	key = to_string(desc.DepthEnable) + token
		+ to_string(desc.DepthWriteMask) + token
		+ to_string(desc.DepthFunc) + token
		+ to_string(desc.StencilEnable) + token
		+ to_string(desc.StencilReadMask) + token
		+ to_string(desc.StencilWriteMask) + token
		+ to_string(desc.FrontFace.StencilFailOp) + token
		+ to_string(desc.FrontFace.StencilDepthFailOp) + token
		+ to_string(desc.FrontFace.StencilPassOp) + token
		+ to_string(desc.FrontFace.StencilFunc) + token
		+ to_string(desc.BackFace.StencilFailOp) + token
		+ to_string(desc.BackFace.StencilDepthFailOp) + token
		+ to_string(desc.BackFace.StencilPassOp) + token
		+ to_string(desc.BackFace.StencilFunc);

	if (depthStencilStates.find(key) == depthStencilStates.end())//�ش��ϴ� srv�� ������
	{
		HRESULT hr = resources->device->CreateDepthStencilState(&desc, &depthStencilStates[key].data);

		assert(SUCCEEDED(hr));
	}//����

	//������ �׳�, ������ ������ ��
	dest.resource = depthStencilStates[key].data;


	dest.Copy = [&, key]()
	{
		// ��� Ƚ�� �÷��ְ�
		depthStencilStates[key].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, key]()
	{
		depthStencilStates[key].refCount--;
		if (depthStencilStates[key].refCount == 0)
		{
			ID3D11DepthStencilState* del = depthStencilStates[key].data;
			depthStencilStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void DepthStencilStateResources::SetDepthEnabled(D3D11_DEPTH_STENCIL_DESC desc)
{
	depthEnabledDesc = desc;
}

void DepthStencilStateResources::SetDepthDisabled(D3D11_DEPTH_STENCIL_DESC desc)
{
	depthDisabledDesc = desc;
}

void DepthStencilStateResources::Release()
{
	for (auto depthStencilState : depthStencilStates)
	{
		depthStencilState.second.data->Release();
	}
}

void BlendStateResources::Get(Resource<ID3D11BlendState>& dest, D3D11_BLEND_DESC desc)
{
	dest.Return();

	string key = "";
	char token = '_';
	key = to_string(desc.RenderTarget[0].BlendEnable) + token
		+ to_string(desc.RenderTarget[0].SrcBlend) + token
		+ to_string(desc.RenderTarget[0].DestBlend) + token
		+ to_string(desc.RenderTarget[0].SrcBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].DestBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].BlendOpAlpha) + token
		+ to_string(desc.RenderTarget[0].RenderTargetWriteMask);

	if (blendStates.find(key) == blendStates.end())//�ش��ϴ� �����Ͱ� ������ ���� ����
	{
		HRESULT hr = resources->device->CreateBlendState(&desc, &blendStates[key].data);

		assert(SUCCEEDED(hr));
	}//����

	//������ �׳�, ������ ������ ��
	dest.resource = blendStates[key].data;

	dest.Copy = [&, key]()
	{
		// ��� Ƚ�� �÷��ְ�
		blendStates[key].refCount++;
	};
	dest.Copy();

	//���ҽ� ��ȯ ����
	dest.Ret = [&, key]()
	{
		blendStates[key].refCount--;
		if (blendStates[key].refCount == 0)
		{
			ID3D11BlendState* del = blendStates[key].data;
			blendStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void BlendStateResources::SetDefault(D3D11_BLEND_DESC desc)
{
	defaultDesc = desc;
}


void BlendStateResources::Release()
{
	for (auto blendState : blendStates)
		blendState.second.data->Release();
}

void Texture2DResources::Create(Resource<ID3D11Texture2D>& dest, D3D11_TEXTURE2D_DESC desc, const D3D11_SUBRESOURCE_DATA* pInitData)
{
	dest.Return();

	ID3D11Texture2D* data;

	HRESULT hr = resources->device->CreateTexture2D(&desc, pInitData, dest);

	assert(SUCCEEDED(hr));

	texture2Ds[dest].data = dest;

	dest.Copy = [&]()
	{
		texture2Ds[dest].refCount++;
	};
	dest.Copy();

	dest.Ret = [&]()
	{
		texture2Ds[dest].refCount--;
		if (texture2Ds[dest].refCount == 0)
		{
			ID3D11Texture2D* del = texture2Ds[dest].data;
			if (texture2Ds.find(del) != texture2Ds.end())
				texture2Ds.erase(del);
			SAFE_RELEASE(del);
		}
	};
}

void Texture2DResources::Release()
{
	for (auto texture2D : texture2Ds)
		SAFE_RELEASE(texture2D.second.data);
}

void RenderTargetViewResources::Create(Resource<ID3D11RenderTargetView>& dest, D3D11_RENDER_TARGET_VIEW_DESC desc, ID3D11Resource* buffer)
{
	dest.Return();

	HRESULT hr = resources->device->CreateRenderTargetView(buffer, &desc, dest);

	assert(SUCCEEDED(hr));

	rtvs[dest].data = dest;

	dest.Copy = [&]()
	{
		rtvs[dest].refCount++;
	};
	dest.Copy();

	dest.Ret = [&]()
	{
		rtvs[dest].refCount--;
		if (rtvs[dest].refCount == 0)
		{
			ID3D11RenderTargetView* del = rtvs[dest].data;
			if (rtvs.find(del) != rtvs.end())
				rtvs.erase(del);
			SAFE_RELEASE(del);
		}
	};
}
//RTT Ŭ���� ���ο��� �ѹ� �� �������ִ� �ɷ� ����

void RenderTargetViewResources::Release()
{
	for (auto rtv : rtvs)
		SAFE_RELEASE(rtv.second.data);
}

void DepthStencilViewResources::Create(Resource<ID3D11DepthStencilView>& dest, D3D11_DEPTH_STENCIL_VIEW_DESC desc, ID3D11Resource* buffer)
{
	dest.Return();

	HRESULT hr = resources->device->CreateDepthStencilView(buffer, &desc, dest);

	assert(SUCCEEDED(hr));

	depthStencilViews[dest].data = dest;

	dest.Copy = [&]()
	{
		depthStencilViews[dest].refCount++;
	};
	dest.Copy();

	dest.Ret = [&]()
	{
		depthStencilViews[dest].refCount--;
		if (depthStencilViews[dest].refCount == 0)
		{
			ID3D11DepthStencilView* del = depthStencilViews[dest].data;
			if (depthStencilViews.find(del) != depthStencilViews.end())
				depthStencilViews.erase(del);
			SAFE_RELEASE(del);
		}
	};
}

void DepthStencilViewResources::Release()
{
	for (auto rtv : depthStencilViews)
		SAFE_RELEASE(rtv.second.data);
}

void BufferResources::Create(Resource<ID3D11Buffer>& dest, D3D11_BUFFER_DESC desc, const D3D11_SUBRESOURCE_DATA* pInitData)
{
	dest.Return();

	ID3D11Texture2D* data;

	HRESULT hr = resources->device->CreateBuffer(&desc, pInitData, dest);

	assert(SUCCEEDED(hr));

	buffers[dest].data = dest;

	dest.Copy = [&]()
	{
		buffers[dest].refCount++;
	};
	dest.Copy();

	dest.Ret = [&]()
	{
		buffers[dest].refCount--;
		if (buffers[dest].refCount == 0)
		{
			ID3D11Buffer* del = buffers[dest].data;
			if (buffers.find(del) != buffers.end())
				buffers.erase(del);
			SAFE_RELEASE(del);
		}
	};
}

void BufferResources::Release()
{
	for (auto buffer : buffers)
		SAFE_RELEASE(buffer.second.data);
}
