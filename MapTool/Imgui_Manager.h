#pragma once

#include "pch.h"
#include "MapObjectScript.h"

class ImGui_Manager
{
	DECLARE_SINGLETON(ImGui_Manager)

private:
	enum GizmoOp { GizmoTR, GizmoRT, GizmoSC, GizmoEND };
	// 기즈모를 띄울 대상 ( 맵오브젝트 or 점광원 )
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
	// 설치할 수 있는 오브젝트 목록과 모양을 보여주고 선택하는 프레임.
	void Frame_ObjectBase();
	// 설치할 수 있는 오브젝트 목록과 모양을 보여주고 선택하는 프레임.
	void Frame_ObjectBaseManager();
	// 현재 배치되어있는 오브젝트 목록 프레임.
	void Frame_Objects();
	// 현재 배치되어있는 오브젝트 정보 프레임
	void Frame_SelcetObjectManager();
	// 광원의 정보 관리.
	void Frame_Light();
	// 벽의 정보 관리
	void Frame_Wall();
	// 쉐이더옵션관리
	void Frame_ShaderOption();
	// 터레인 관련
	void Frame_Terrain();
	// 피킹관리
	void Frame_PickingManager();

	// 피킹
	void Picking_Object();
	// 샘플오브젝트로 시야이동
	void LookAtSampleObject();

	// 스카이박스목록 불러오기
	HRESULT Load_SkyBoxTexture();
	// 맵오브젝트베이스 목록 불러오기
	HRESULT Load_MapObjectBase();
	// 타일목록 및 텍스쳐로드
	void Load_TerrainTile();

	// 선택한 맵 오브젝트 생성
	HRESULT Create_SelectObject();
	// 맵 오브젝트 생성정보를 바탕으로 오브젝트를 생성하여 반환하는 함수
	shared_ptr<GameObject> Create_MapObject(MapObjectScript::MapObjectDesc _CreateDesc);
	// 현재 정보로 점광원 생성
	HRESULT Create_SelectPointLight();
	// 빛정보를 바탕으로 생성 후 씬에 추가 후 포인터 반환
	shared_ptr<GameObject> Create_PointLight(LightInfo _ptltInfo);
	// 벽 메시 생성 후 멤버변수에 적용
	void Create_WallMesh();
	// 바닥 메시 생성 후 멤버변수에 적용
	void Create_GroundMesh();
	// 벽메시 정보배열 초기화
	void Clear_WallMesh();
	// 최근에 피킹한벽 삭제
	void Delete_WallMesh();
	// 최근에 피킹한바닥 삭제
	void Delete_GroundMesh();
	// 플레이어 생성위치를 현재의 카메라 포지션으로 변경
	void SetPlayerPosByCameraPos();
	// 플레이어 룩앳 위치를 피킹 포지션으로 변경
	void SetPlayerLookAtPosByPickingPos();

private:
// 터레인
	// 현재 갖고있는 타일정보로 터레인새로생성
	void Create_Terrain();
	// 터레인정보(메시)를 기반으로 터레인오브젝트 생성
	void Create_Terrain(shared_ptr<class Terrain> _pTerrainMesh, _int _iTerrainSizeX, _int _iTerrainSizeY);
	// 마스크텍스쳐 생성저장
	void Create_MaskTexture();
	// 범위에 들어가는 정점들의 높이를 fInitHeight로 변경
	void Set_TerrainHeight();

// 물 생성
	void Load_Water();
	// 점광원 제거
	HRESULT Delete_PointLight();
	// 맵오브젝트 제거
	HRESULT Delete_MapObject();
	// 맵오브젝트 저장
	HRESULT Save_MapObject();
	// 맵오브젝트 불러오기
	HRESULT Load_MapObject();
	// 맵에있는 설치된 오브젝트 이름을 모은 텍스트 파일 저장
	HRESULT Save_ModelNames();
	// 지형정보 저장.
	HRESULT Save_TerrainData();
	// 지형정보 가져와서 멤버변수 메시렌더러에 넣고 터레인 다시생성하기
	HRESULT LoadAndCreateTerrain();

	void Cal_NormalTangent();

	// 오브젝트를 받아와서 컬링포지션과 길이를 계산하여 반영, 컬링포지션과 길이를 float4로 반환
	_float4 Compute_CullingData(shared_ptr<GameObject>& _pGameObject);
	
	// 그림자, 블러, 컬링계산
	void Bake(shared_ptr<GameObject>& _pGameObject);
	void BakeAll();
	
	// 베이스오브젝트이름을 가지고 샘플 오브젝트 생성
	void Create_SampleObjects();
	// 현재 선택한 베이스오브젝트로 샘플 맵오브젝트 모델변경 및 카메라 조정
	void Set_SampleObject();
    // 윤성이형의 셰이더옵션변경탭
	void RenderOptionTab();
	// 모든 쉐이더 옵션 초기화
	void ClearAllShaderOptions();

private:
// 설치 가능한 오브젝트 목록
	vector<const char*> m_strObjectBaseNameList;
	//// 같은이름의 모델의 중복개수(모델별 번호)
	//vector<_uint> m_iObjectBaseIndexList;
	// 로드시 문자열 동적할당을 위한 공간
	vector<shared_ptr<char[]>> m_strObjectBaseNamePtr;
	// 현재 선택된 베이스 오브젝트의 번호
	_int m_iObjectBaseIndex = { 0 };
	// 오브젝트 생성시 정보
	MapObjectScript::MAPOBJDESC m_CreateObjectDesc;
	// 베이스오브젝트 필터
	char m_szBaseObjectFilter[MAX_PATH] = "";
	_int m_iFilteredBaseObjectsIndex = { 0 };
	// 필터링이 완료된 이름모음
	vector<const char*> m_FilteredBaseObjectNames;
	// 필터링이 완료된 이름모음주소공간
	vector<shared_ptr<char[]>> m_strFilteredNamePtr;
	// 베이스오브젝트리스트가 바뀌었을때 높이를 찾아가도록 변경, true면 틱에서 1회변경후 false로 다시바꿈
	_bool m_bBaseObjectListResetHeight = { false };
	// 오브젝트 피킹가능상태
	_bool m_bMapObjectPickingMode = { true };

// 샘플오브젝트
	shared_ptr<GameObject> m_SampleObject;
	// 샘플이동전 카메라의 상태
	_float4x4 m_matPreCamera = { XMMatrixIdentity() };
	_float m_fSampleModelCullSize = { 0.f };

// 설치된 오브젝트 목록
	vector<const char*> m_strObjectName;
	// Load한 이름들을 동적할당하여 소유하고있을 포인터벡터
	vector<shared_ptr<char[]>> m_strObjectNamePtr;
	// 현재옵젝번호
	_int m_iObjects = { 0 };
	// 현재 설치된 오브젝트들을 가지는 셰어드포인터
	vector<shared_ptr<GameObject>> m_pMapObjects;
	_float3 m_PickingPos = { 0.f, 0.f, 0.f };
	// 세이브파일이름
	char m_szSaveFileName[MAX_PATH] = "";
	// 콜라이더목록
	const char* m_szColliderTypes[4] = { "Sphere","AABB", "OBB", "Mesh" };
	// 설치된 오브젝트리스트가 바뀌었을때 높이를 찾아가도록 변경, true면 틱에서 1회변경후 false로 다시바꿈
	_bool m_bObjectListResetHeight = { false };

// 기즈모관련
	GizmoOp m_eGizmoOp = { GizmoTR };
	_bool	m_bGizmoOp[GizmoEND] = { false, };
	GizmoTarget m_GizmoTarget = GizmoTMapObj;

// 스카이박스 관련
	_float4 m_CamPos = { 0.f, 0.f, 0.f, 0.f };
	vector<const char*> m_strSkyboxList;
	// 로드시 문자열 동적할당을 위한 공간
	vector<shared_ptr<char[]>> m_strSkyBoxNamePtr;
	_int m_iCurrentSkyBoxIndex = { 0 };

// 빛관련
	_float4 m_DirectionalLightPos = _float4{ 0.f, 0.f, 0.f, 1.f };
	_float3 m_DirectionalLightLookAtPos = _float3{ 0.f, 0.f, 0.f};
	LightInfo m_DirectionalLightInfo; // 빛 생성정보
	vector<shared_ptr<GameObject>> m_pPointLightObjects;
	vector<const char*> m_strPointLightList;
	_int m_iPointLightIndex = { 0 };
	// 점광원 생성정보
	LightInfo m_CreatePointLightInfo;
	// 생성할 점광원의 이펙트 소지여부
	_bool m_bPointLightEffectHas = { false };
	_float4 m_CreateTargetAmbient = { 1.f, 1.f, 1.f, 1.f };
	_float4 m_CreateTargetDiffuse = { 1.f, 1.f, 1.f, 1.f };
	_float m_fCreatePointLightSpeed = { 1.f };

// 벽을위한피킹정보
	// 벽,바닥피킹선택
	_bool m_bWallPickingMod = { true };
	_float3 m_WallPickingPos[2] = { _float3{0.f, 0.f, 0.f}, _float3{0.f, 0.f, 0.f} };
	_bool m_bFirstWallPick = { true };
	_float m_fWallHeight = { 10.f };
	// 좌하단 우상단 페어(사각형)를 가진 벽을 만들기 위한 벡터
	vector<pair<_float3, _float3>> m_WallRectPosLDRU;
// 바닥을위한피킹정보
	_bool m_bGroundPickingMod = { false };
	_float3 m_GroundPickingPos[2] = { _float3{0.f, 0.f, 0.f}, _float3{0.f, 0.f, 0.f} };
	_bool m_bFirstGroundPick = { true };
	// 좌상단 우하단 페어(사각형)를 가진 바닥을 만들기 위한 벡터
	vector<pair<_float3, _float3>> m_GroundRectPosLURD;

// 플레이어 위치
	_float4 m_PlayerCreatePosition = { 0.f, 0.f, 0.f, 1.f };
	_float4 m_PlayerLookAtPosition = { 0.f, 0.f, 0.f, 1.f };

// 맵 로드정보
	_int curMapIndex = 0;
	vector<string> m_MapNames;

// 터레인관련
	// 터레인 가로세로 타일개수
	_int m_iTerrainSize[2] = { 1, 1 };
	// 터레인 포인터
	shared_ptr<GameObject> m_pTerrain = { nullptr };
	// 터레인브러시크기
	_float m_fTerrainBrushRadius = { 0.f };
	_float3 m_vTerrainBrushPosition = _float3{ 0.f, 0.f, 0.f};
	// 터레인만 피킹함.
	_bool m_bTerrainPickingMode = { false };
	vector<string> m_TileNames;
	_int m_iCurrentTile = { 0 };
	_float m_fTilePressForce = { 0.01f };
	// 범위안에 들어오는 녀석들의 높이를 해당숫자로 초기화 
	_float m_fTerrainSetHeight = { 0.f };

// 바다
	// 물색깔
	Color m_WaterColor1 = Color(-36.f / 255.f, 124.f / 255.f, 1.f, 1.f);
	Color m_WaterColor2 = Color(-69.f / 255.f, 62.f / 255.f, 1.f, 1.f);
	// 워터 림
	_int m_WaterInt0 = { 100 };
	_int m_WaterInt1 = { 10 };

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