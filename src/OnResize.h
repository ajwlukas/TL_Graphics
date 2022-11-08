#pragma once

#include <stdint.h>
#include <vector>

class OnResize_Observer
{
private:
	friend class OnResizeNotice;
	virtual void  OnResize(uint32_t width, uint32_t height) = 0;
};

class OnResizeNotice
{
	using Observer = OnResize_Observer;
public:
	void AddObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void SubObserver(Observer* observer)
	{
		observers.erase(find(observers.begin(), observers.end(), observer));
	}

	void Update(uint32_t width, uint32_t height)
	{
		if (this->width != width || this->height != height)
		{
			this->width = width;
			this->height = height;

			OccurEvent();
		}
	}

	uint32_t GetWidth() { return width; }
	uint32_t GetHeight() { return height; }

private:

	void OccurEvent()
	{
		if (width == 0 && height == 0) return;

		for (Observer* ob : observers)
			ob->OnResize(width, height);
	}

	//���� 0�̸� dll�� �ʱ�ȭ ������ �����ϰ� �������ؼ� �γ��ϰ� �ٲ���
	uint32_t width = 1;
	uint32_t height = 1;

	std::vector<Observer*> observers;
};
