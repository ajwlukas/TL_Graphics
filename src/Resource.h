#pragma once

#include <functional>

/// Resource<ResourceType>���� ���, ��� �� Return() ���� ��
/// type ���ڷδ� ID3D �ø���, delete�� �ƴ� Release ���� �ָ� ���� ��
/// written by Ssreppa 22.06.28
/// 
/// Return() �ڵ����� ����, Leak���� ���� Free
/// Copy()�� refCount �ڵ����� �÷���
/// �ƹ����Գ� �ᵵ ���� ���� ��
/// written by Ssreppa 22.07.01

template <typename T>
class Resource
{
public:
	Resource() : resource(nullptr), Ret([]() {}), Copy([]() {}) {}
	~Resource()
	{
		Ret();
	}

	void* operator new(std::size_t size) = delete;//�����Ҵ��ؼ� ����ϴ� �� ����, ����ڰ� delete ����� ��츦 ���

	Resource(const Resource& other)
	{
		if (this == &other) return;
		this->Copy = other.Copy;
		this->Ret = other.Ret;
		this->Copy();
		this->resource = other.resource;
	}

	void operator = (const Resource& other)
	{
		if (this == &other) return;
		this->Ret();
		this->Copy = other.Copy;
		this->Ret = other.Ret;
		this->Copy();
		this->resource = other.resource;
	}

	operator T* () { return resource; }
	operator T** () { return &resource; }

	T* resource;

	void Release() = delete;

	void Return()
	{
		Ret();
		Ret = []() {};
		Copy = []() {};
	}

	//�̰� �����ְ� ������ friend �ɱ� �� ������ �̷��, ����ڴ� ������� �� �� �ϴ�.
	std::function<void()> Copy;//ex) shared count increse��
	std::function<void()> Ret;
};
