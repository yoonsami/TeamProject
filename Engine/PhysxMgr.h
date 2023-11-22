#pragma once


enum class PxLayerFilter : _uint
{
	Player = (1 << 0),
	Monster = (1 << 1),
	Terrain = (1 << 2),
	Obstacle = (1 << 3),
	Camera = (1 << 4)
};

class PhysxMgr
{
	DECLARE_SINGLETON(PhysxMgr)
public:
	~PhysxMgr();
public:
	HRESULT Init();
	void Tick();



public:
	PxPhysics* Get_PxPhysics() { return m_pPhysics; }
	PxScene* Get_PxScene() { return m_pScene; }
	PxCookingParams Get_Cooking() { return PxCookingParams(m_ToleranceScale); }
	PxControllerManager* Get_ControllManager() { return m_pManager; }
	void Add_GameObject(shared_ptr<GameObject> obj) { m_PhysxObject.push_back(obj); }
	void Set_CharacterControllerNull();

private:
	PxDefaultAllocator m_DefalutAllocatorCallback;
	PxDefaultErrorCallback m_DefaultErrorCallback;
	PxDefaultCpuDispatcher* m_pDistpatcher = nullptr;
	PxTolerancesScale	m_ToleranceScale;
	PxFoundation* m_pFoundation = nullptr;
	PxPhysics* m_pPhysics = nullptr;
	PxScene* m_pScene = nullptr;
	PxPvd* m_pPvd = nullptr;
	PxControllerManager* m_pManager = nullptr;
	PxPvdTransport* m_pTransport = nullptr;

	vector<weak_ptr<GameObject>> m_PhysxObject;
};

