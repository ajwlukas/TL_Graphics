#pragma once

struct Instantiateds
{
	vector<Instance*> partInstances;
	vector<Instance*> boneInstances;
};
/// <summary>
/// 
/// InstancingObject ������ ������ �ִ� �ֵ�, ex) ���� 4��Ʈ
/// Bone���� InstancingObject
/// 
/// �̳༮�� ������ �̿��� ���ο� �ָ� ����ش�.
/// 
/// </summary>

class Proto
{
//{
//public:
//	Proto();
//	~Proto();
//
//	void Update();
//	void Render();
//
//	string name;
//
//	vector<InstancingObject*> parts;
//	vector<InstancingObject*> bones;
//
//	UINT FindIDByTransform(const Transform* t);
//
//	void DeleteInstance(const Transform* t);
//
//	void SetCurrentAnimation(const Transform* t, string clipName);
//
//	vector<int> parentInfo;
//	// parentInfo.size() == parts.size() + bones.size(), 
//	//ex) parts size == 2, bones size == 1;
//	//parentInfo[0] = 2 �̸� , part[0]�� �θ� bones[0]
//	//parentInfo[2] = 1 �̸�,  bones[0]�� �θ� parts[1]
//
//	void Create(Transform* transform);//todo : ����°� �ڵ� ����� ��� ��
//	void Create(Transform* transform, Vector3 posOffset, Vector3 rotOffset);//todo : ����°� �ڵ� ����� ��� ��
//	//testCode
//	vector<Instantiateds*> instantiateds;
//
//	vector<Transform*> ts;
//
//
//	void Clear();
};

