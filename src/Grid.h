#pragma once

/// <summary>
/// ī�޶��� x, z ��ǥ�� ����ٴϴ� 2���� ��ü,
/// ���̴����� ������ ���� pos.x % 10 ==0, pos.y % 10 == 0, �϶� �ȼ��� ĥ���ִ� ������ ����
/// 
/// written by Ssreppa 22.06.22
/// </summary>

class Camera;

class Grid : public Object
{
public:
	Grid(Camera* camera);

	virtual void Update() override;
	virtual void Render() override;

private:
	void CreateGrid();

	Camera* cam;
};