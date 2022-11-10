#pragma once
#define MAX_INSTANTS_SIZE 100
#define MAX_BONE_SIZE 100
/// <summary>
/// ������Ʈ���� Instants�� �������� �ߴ�.
/// CreateInstance�� ���� Instance*�� ���Ϲ޾� ������ �� �ְԲ� �������.
/// SRV�� �����͸� �־� ����Ѵ�.
/// </summary>
//�ν��Ͻ��� �� ������


class IInstancingBuffer abstract
{
//public:
//	void UpdateData(UINT instanceID, void* src)
//	{
//		memcpy(reinterpret_cast<char*>(data) + instanceID * dataSize, src, dataSize);
//	}
//	
//	
//	Resource<ID3D11Buffer>& Buffer() { return buffer; }
//	Resource<ID3D11ShaderResourceView>& SrvBuffer() { return srvBuffer; }
//
//	void Set()
//	{
//
//
//		D3D11_MAPPED_SUBRESOURCE mappedResource;
//		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
//
//		DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//
//		// Copy Resource Data..
//		memcpy(mappedResource.pData, data, dataSize * 100);
//
//		// GPU Access UnLock Buffer Data..
//		DC->Unmap(buffer, 0);
//
//		//DC->UpdateSubresource(buffer, 0, 0, data, 0, 0);
//		DC->VSSetShaderResources(slot, 1, srvBuffer);
//	}
//
//protected:
//	void* data;//���� ������ ����, �޸𸮿� ���������� ��ġ��Ű�� ���ؼ�
//	size_t dataSize;
//	UINT slot;
//	Resource<ID3D11Buffer> buffer;//������Ʈ��
//	Resource<ID3D11ShaderResourceView> srvBuffer;//buffer�� �����ִ�
};

//template<typename T, UINT SLOT>
//class InstancingBuffer : public IInstancingBuffer
//{
//public:
//	InstancingBuffer()
//	{
//		data = new T[MAX_INSTANTS_SIZE];
//
//		slot = SLOT;
//		dataSize = sizeof(T);
//
//		D3D11_BUFFER_DESC desc = {};
//		desc.Usage = D3D11_USAGE_DYNAMIC;
//		desc.ByteWidth = sizeof(T) * MAX_INSTANTS_SIZE;
//		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//		desc.MiscFlags = 0;
//		desc.StructureByteStride = 0;
//
//		D3D11_SUBRESOURCE_DATA initData;
//		initData.pSysMem = data;
//
//		RESOURCES->buffers->Create(buffer, desc, &initData);
//
//		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//		srvDesc.Buffer.FirstElement = 0;
//		srvDesc.Buffer.NumElements = sizeof(T) * MAX_INSTANTS_SIZE / 16;//1������Ʈ�� ũ�� = sizeof(float4) = 16
//
//		RESOURCES->srvs->Create(srvBuffer, srvDesc, buffer);
//	}
//	~InstancingBuffer()
//	{
//		delete[] data;
//	}
//
//};
//
//struct World
//{
//	World() : world(XMMatrixIdentity())
//	{
//
//	}
//	Matrix world;
//};
//
//struct Bones
//{
//	Bones() : bones{}
//	{
//
//	}
//	Matrix bones[MAX_BONE_SIZE];
//};
//
////�ν��Ͻ� �ϳ��� ����
//class Instance
//{
//public:
//	Instance() : instanceNum(0), animator(nullptr)//, skinner(nullptr)
//	{}
//	~Instance() 
//	{
//		if (animator != nullptr) delete animator;
//		//if (skinner != nullptr) delete skinner;
//	}
//
//	void Update()
//	{
//		if (animator != nullptr)//������ animator�� ������ �ִٸ�
//			animator->Update();
//
//		//if (skinner != nullptr)//todo: ���� �޾��� ��, skinning���� �ֵ鸸 ������
//		//	skinner->Update();
//
//		transform.UpdateWorld();
//	}
//
//	Transform transform;
//	UINT instanceNum;
//
//	Animator* animator;
//	//Skinner* skinner;
//
//	vector<void*> dataAddress;
//};

//�ν��Ͻ����� ����, �ν��Ͻ����� �����͸� �����Ѵ�.
class Instants
{
public:
//	Instants(InstancingObject* proto);
//	~Instants();
//
//	void Update();
//	void Render();
//
//	void Clear();
//
//	Instance* CreateInstance();
//
//	void DeleteInstance(const UINT instanceNum);
//	void DeleteInstance(const Transform* t);// todo : ���� �ȵǴ� �ڵ�����... �ϴ� ������Ʈ �ؾ��ϴ�...
//	void SetCurrentAnimation(const UINT instanceNum, string clipName);
//
//
//	UINT GetCorrespondingInstanceNum(const Transform* t);
//
//	template<typename T, UINT SLOT>
//	void AddBuffer();
//
//private:
//	InstancingObject* proto;
//
//	vector<Instance*> instants;
//
//public:
//
//	vector<IInstancingBuffer*> instanceBuffers;
//
//	UINT count;
};

//template<typename T, UINT SLOT>
//inline void Instants::AddBuffer()
//{
//	instanceBuffers.emplace_back(new InstancingBuffer<T, SLOT>());
//}

