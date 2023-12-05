#pragma once

class GameObject;
class Camera;

class PickingMgr
{
	DECLARE_SINGLETON(PickingMgr)

public:
	shared_ptr<GameObject> Pick_Collider(_float2 screenPos, shared_ptr<Camera> camera, const vector<shared_ptr<GameObject>>& gameObjects, OUT _float3& outPos);
	shared_ptr<GameObject> Pick_Mesh(_float2 screenPos, shared_ptr<Camera> camera, const list<shared_ptr<GameObject>>& gameObjects, OUT _float3& outPos);
	// �ʿ�����Ʈ�� ��ŷ�ϱ� ���� �Լ�
	shared_ptr<GameObject> Pick_Mesh(_float2 screenPos, shared_ptr<Camera> camera, vector<shared_ptr<GameObject>>& gameObjects, OUT _float3& outPos);
	// Ư��������Ʈ�� ��ŷ�ϱ� ���� �Լ� ex) Terrain
	shared_ptr<GameObject> Pick_Mesh(_float2 screenPos, shared_ptr<Camera> camera, shared_ptr<GameObject> gameObject, OUT _float3& outPos);
	void Brushing(_float2 screenPos, shared_ptr<Camera> camera, shared_ptr<GameObject> gameObject,_float radius, ID3D11Texture2D** texture, vector<_ulong>& pixels);

};

