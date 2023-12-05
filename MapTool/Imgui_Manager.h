#pragma once

#include "pch.h"
#include "MapObjectScript.h"

class ImGui_Manager
{
	DECLARE_SINGLETON(ImGui_Manager)

private:
	enum GizmoOp { GizmoTR, GizmoRT, GizmoSC, GizmoEND };
	// ����� ��� ��� ( �ʿ�����Ʈ or ������ )
	enum GizmoTarget { GizmoTMapObj, GizmoTPointLight,GizmoAnimNpc, GizmoTEnd };

private:
	virtual ~ImGui_Manager();

private:
	bool show_demo_window = true;
	bool show_another_window = false;

public:
	void ImGui_SetUp();
	void ImGui_Tick();
	void ImGui_Render();

private:
	void Show_Gizmo();
	// ��ġ�� �� �ִ� ������Ʈ ��ϰ� ����� �����ְ� �����ϴ� ������.
	void Frame_ObjectBase();
	// ��ġ�� �� �ִ� ������Ʈ ��ϰ� ����� �����ְ� �����ϴ� ������.
	void Frame_ObjectBaseManager();
	// ���� ��ġ�Ǿ��ִ� ������Ʈ ��� ������.
	void Frame_Objects();
	// ���� ��ġ�Ǿ��ִ� ������Ʈ ���� ������
	void Frame_SelcetObjectManager();
	// ������ ���� ����.
	void Frame_Light();
	// ���� ���� ����
	void Frame_Wall();
	// ���̴��ɼǰ���
	void Frame_ShaderOption();
	// �ͷ��� ����
	void Frame_Terrain();
	// ��ŷ����
	void Frame_PickingManager();

	// ��ŷ
	void Picking_Object();
	// ���ÿ�����Ʈ�� �þ��̵�
	void LookAtSampleObject();

	// ��ī�̹ڽ���� �ҷ�����
	HRESULT Load_SkyBoxTexture();
	// �ʿ�����Ʈ���̽� ��� �ҷ�����
	HRESULT Load_MapObjectBase();
	// Ÿ�ϸ�� �� �ؽ��ķε�
	void Load_TerrainTile();

	// ������ �� ������Ʈ ����
	HRESULT Create_SelectObject();
	// �� ������Ʈ ���������� �������� ������Ʈ�� �����Ͽ� ��ȯ�ϴ� �Լ�
	shared_ptr<GameObject> Create_MapObject(MapObjectScript::MapObjectDesc _CreateDesc);
	// ���� ������ ������ ����
	HRESULT Create_SelectPointLight();
	// �������� �������� ���� �� ���� �߰� �� ������ ��ȯ
	shared_ptr<GameObject> Create_PointLight(LightInfo _ptltInfo);
	// �� �޽� ���� �� ��������� ����
	void Create_WallMesh();
	// �ٴ� �޽� ���� �� ��������� ����
	void Create_GroundMesh();
	// ���޽� �����迭 �ʱ�ȭ
	void Clear_WallMesh();
	// �ֱٿ� ��ŷ�Ѻ� ����
	void Delete_WallMesh();
	// �ֱٿ� ��ŷ�ѹٴ� ����
	void Delete_GroundMesh();
	// �÷��̾� ������ġ�� ������ ī�޶� ���������� ����
	void SetPlayerPosByCameraPos();
	// �÷��̾� ��� ��ġ�� ��ŷ ���������� ����
	void SetPlayerLookAtPosByPickingPos();

private:
// �ͷ���
	// ���� �����ִ� Ÿ�������� �ͷ��λ��λ���
	void Create_Terrain();
	// �ͷ�������(�޽�)�� ������� �ͷ��ο�����Ʈ ����
	void Create_Terrain(shared_ptr<class Terrain> _pTerrainMesh, _int _iTerrainSizeX, _int _iTerrainSizeY);
	// ����ũ�ؽ��� ��������
	void Create_MaskTexture();
	// ������ ���� �������� ���̸� fInitHeight�� ����
	void Set_TerrainHeight();

// �� ����
	void Load_Water();
	// ������ ����
	HRESULT Delete_PointLight();
	// �ʿ�����Ʈ ����
	HRESULT Delete_MapObject();
	// �ʿ�����Ʈ ����
	HRESULT Save_MapObject();
	// �ʿ�����Ʈ �ҷ�����
	HRESULT Load_MapObject();
	// �ʿ��ִ� ��ġ�� ������Ʈ �̸��� ���� �ؽ�Ʈ ���� ����
	HRESULT Save_ModelNames();
	// �������� ����.
	HRESULT Save_TerrainData();
	// �������� �����ͼ� ������� �޽÷������� �ְ� �ͷ��� �ٽû����ϱ�
	HRESULT LoadAndCreateTerrain();

	// ������Ʈ�� �޾ƿͼ� �ø������ǰ� ���̸� ����Ͽ� �ݿ�, �ø������ǰ� ���̸� float4�� ��ȯ
	_float4 Compute_CullingData(shared_ptr<GameObject>& _pGameObject);
	
	// �׸���, ��, �ø����
	void Bake(shared_ptr<GameObject>& _pGameObject);
	void BakeAll();
	
	// ���̽�������Ʈ�̸��� ������ ���� ������Ʈ ����
	void Create_SampleObjects();
	// ���� ������ ���̽�������Ʈ�� ���� �ʿ�����Ʈ �𵨺��� �� ī�޶� ����
	void Set_SampleObject();
    // ���������� ���̴��ɼǺ�����
	void RenderOptionTab();
	// ��� ���̴� �ɼ� �ʱ�ȭ
	void ClearAllShaderOptions();

private:
// ��ġ ������ ������Ʈ ���
	vector<const char*> m_strObjectBaseNameList;
	//// �����̸��� ���� �ߺ�����(�𵨺� ��ȣ)
	//vector<_uint> m_iObjectBaseIndexList;
	// �ε�� ���ڿ� �����Ҵ��� ���� ����
	vector<shared_ptr<char[]>> m_strObjectBaseNamePtr;
	// ���� ���õ� ���̽� ������Ʈ�� ��ȣ
	_int m_iObjectBaseIndex = { 0 };
	// ������Ʈ ������ ����
	MapObjectScript::MAPOBJDESC m_CreateObjectDesc;
	// ���̽�������Ʈ ����
	char m_szBaseObjectFilter[MAX_PATH] = "";
	_int m_iFilteredBaseObjectsIndex = { 0 };
	// ���͸��� �Ϸ�� �̸�����
	vector<const char*> m_FilteredBaseObjectNames;
	// ���͸��� �Ϸ�� �̸������ּҰ���
	vector<shared_ptr<char[]>> m_strFilteredNamePtr;
	// ���̽�������Ʈ����Ʈ�� �ٲ������ ���̸� ã�ư����� ����, true�� ƽ���� 1ȸ������ false�� �ٽùٲ�
	_bool m_bBaseObjectListResetHeight = { false };
	// ������Ʈ ��ŷ���ɻ���
	_bool m_bMapObjectPickingMode = { true };

// ���ÿ�����Ʈ
	shared_ptr<GameObject> m_SampleObject;
	// �����̵��� ī�޶��� ����
	_float4x4 m_matPreCamera = { XMMatrixIdentity() };
	_float m_fSampleModelCullSize = { 0.f };

// ��ġ�� ������Ʈ ���
	vector<const char*> m_strObjectName;
	// Load�� �̸����� �����Ҵ��Ͽ� �����ϰ����� �����ͺ���
	vector<shared_ptr<char[]>> m_strObjectNamePtr;
	// ���������ȣ
	_int m_iObjects = { 0 };
	// ���� ��ġ�� ������Ʈ���� ������ �ξ��������
	vector<shared_ptr<GameObject>> m_pMapObjects;
	_float3 m_PickingPos = { 0.f, 0.f, 0.f };
	// ���̺������̸�
	char m_szSaveFileName[MAX_PATH] = "";
	// �ݶ��̴����
	const char* m_szColliderTypes[4] = { "Sphere","AABB", "OBB", "Mesh" };
	// ��ġ�� ������Ʈ����Ʈ�� �ٲ������ ���̸� ã�ư����� ����, true�� ƽ���� 1ȸ������ false�� �ٽùٲ�
	_bool m_bObjectListResetHeight = { false };

// ��������
	GizmoOp m_eGizmoOp = { GizmoTR };
	_bool	m_bGizmoOp[GizmoEND] = { false, };
	GizmoTarget m_GizmoTarget = GizmoTMapObj;

// ��ī�̹ڽ� ����
	_float4 m_CamPos = { 0.f, 0.f, 0.f, 0.f };
	vector<const char*> m_strSkyboxList;
	// �ε�� ���ڿ� �����Ҵ��� ���� ����
	vector<shared_ptr<char[]>> m_strSkyBoxNamePtr;
	_int m_iCurrentSkyBoxIndex = { 0 };

// ������
	_float4 m_DirectionalLightPos = _float4{ 0.f, 0.f, 0.f, 1.f };
	_float3 m_DirectionalLightLookAtPos = _float3{ 0.f, 0.f, 0.f};
	LightInfo m_DirectionalLightInfo; // �� ��������
	vector<shared_ptr<GameObject>> m_pPointLightObjects;
	vector<const char*> m_strPointLightList;
	_int m_iPointLightIndex = { 0 };
	// ������ ��������
	LightInfo m_CreatePointLightInfo;
	// ������ �������� ����Ʈ ��������
	_bool m_bPointLightEffectHas = { false };
	_float4 m_CreateTargetAmbient = { 1.f, 1.f, 1.f, 1.f };
	_float4 m_CreateTargetDiffuse = { 1.f, 1.f, 1.f, 1.f };
	_float m_fCreatePointLightSpeed = { 1.f };

// ����������ŷ����
	// ��,�ٴ���ŷ����
	_bool m_bWallPickingMod = { true };
	_float3 m_WallPickingPos[2] = { _float3{0.f, 0.f, 0.f}, _float3{0.f, 0.f, 0.f} };
	_bool m_bFirstWallPick = { true };
	_float m_fWallHeight = { 10.f };
	// ���ϴ� ���� ���(�簢��)�� ���� ���� ����� ���� ����
	vector<pair<_float3, _float3>> m_WallRectPosLDRU;
// �ٴ���������ŷ����
	_bool m_bGroundPickingMod = { false };
	_float3 m_GroundPickingPos[2] = { _float3{0.f, 0.f, 0.f}, _float3{0.f, 0.f, 0.f} };
	_bool m_bFirstGroundPick = { true };
	// �»�� ���ϴ� ���(�簢��)�� ���� �ٴ��� ����� ���� ����
	vector<pair<_float3, _float3>> m_GroundRectPosLURD;

// �÷��̾� ��ġ
	_float4 m_PlayerCreatePosition = { 0.f, 0.f, 0.f, 1.f };
	_float4 m_PlayerLookAtPosition = { 0.f, 0.f, 0.f, 1.f };

// �� �ε�����
	_int curMapIndex = 0;
	vector<string> m_MapNames;

// �ͷ��ΰ���
	// �ͷ��� ���μ��� Ÿ�ϰ���
	_int m_iTerrainSize[2] = { 1, 1 };
	// �ͷ��� ������
	shared_ptr<GameObject> m_pTerrain = { nullptr };
	// �ͷ��κ귯��ũ��
	_float m_fTerrainBrushRadius = { 0.f };
	_float3 m_vTerrainBrushPosition = _float3{ 0.f, 0.f, 0.f};
	// �ͷ��θ� ��ŷ��.
	_bool m_bTerrainPickingMode = { false };
	vector<string> m_TileNames;
	_int m_iCurrentTile = { 0 };
	_float m_fTilePressForce = { 0.01f };
	// �����ȿ� ������ �༮���� ���̸� �ش���ڷ� �ʱ�ȭ 
	_float m_fTerrainSetHeight = { 0.f };

// �ٴ�
	// ������
	//Color m_WaterColor1 = Color(100.f / 255.f, 191.f / 255.f, 1.f, 1.f);
	Color m_WaterColor1 = Color(100.f / 255.f, 107.f / 255.f, 202.f / 255.f, 1.f);
	//Color m_WaterColor2 = Color(0.f, 150.f / 255.f, 289.f / 255.f, 1.f);
	Color m_WaterColor2 = Color(0.f, 66.f / 255.f, 236.f / 255.f, 1.f);
	// ��������
	_int m_WaterInt0 = { 3 };
	_int m_WaterInt1 = { 3 };

private:
	void Frame_ModelObj();
	void Show_Models();
	void Show_ModelInfo();
	void Select_ModelAnim();
	void Set_Transform();
	void Save_Files();
	void Load_Files();

	_int m_iCurrentModelIndex = 0;
	_int m_iCurrentObjectIndex = 0;
	_int m_iCurrentFSMIndex = 0;
	_int m_iCurrentAnimIndex = 0;

	struct ObjectMoveInfo
	{
		_bool bMoving = false;
		_int eFSMIndex = 0;
		_float3 minMoveArrayPos = _float3(0.f);
		_float3 maxMoveArrayPos = _float3(0.f);

	};
	vector<pair<shared_ptr<GameObject>,ObjectMoveInfo>> m_pAnimModels;

	weak_ptr<GameObject> m_pControlObjects;

	

};