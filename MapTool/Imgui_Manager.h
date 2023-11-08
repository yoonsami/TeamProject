#pragma once

#include "pch.h"
#include "MapObjectScript.h"

class ImGui_Manager
{
	DECLARE_SINGLETON(ImGui_Manager)

private:
	enum GizmoOp { GizmoTR, GizmoRT, GizmoSC, GizmoEND };

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
	// 현재 배치되어있는 오브젝트 목록 프레임.
	void Frame_Objects();
	// 방향성광원의 정보 관리.
	void Frame_DirectionalLight();
	// 피킹
	void Picking_Object();

	// 맵오브젝트베이스 목록 불러오기
	HRESULT Load_MapObjectBase();
	// 선택한 맵 오브젝트 생성
	HRESULT Create_SelectObject();
	// 맵 오브젝트 생성정보를 바탕으로 오브젝트를 생성하여 반환하는 함수
	shared_ptr<GameObject>& Create_MapObject(MapObjectScript::MapObjectDesc _CreateDesc);

	// 맵오브젝트 제거
	HRESULT Delete_MapObject();
	// 맵오브젝트 저장
	HRESULT Save_MapObject();
	// 맵오브젝트 불러오기
	HRESULT Load_MapObject();

	// 오브젝트를 받아와서 컬링포지션과 길이를 계산하여 반영
	void Compute_CullingData(shared_ptr<GameObject>& _pGameObject);
	
	// 그림자, 블러, 컬링계산
	void Burn(shared_ptr<GameObject>& _pGameObject);
	void BurnAll();

private:
// 설치 가능한 오브젝트 목록
	vector<const char*> m_strObjectBaseNameList;
	// 같은이름의 모델의 중복개수(모델별 번호)
	vector<_uint> m_iObjectBaseIndexList;
	// 로드시 문자열 동적할당을 위한 공간
	vector<shared_ptr<char[]>> m_strObjectBaseNamePtr;
	// 현재 선택된 베이스 오브젝트의 번호
	_int m_iObjectBaseIndex = { 0 };
	// 생성하는 오브젝트에 넣고싶은 UVWeight - UV에 곱해지는 숫자
	//_float m_fUVWeight = { 1.f };
	// 오브젝트 생성시 정보
	MapObjectScript::MAPOBJDESC m_CreateObjectDesc;

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

// 기즈모관련
	GizmoOp m_eGizmoOp = { GizmoTR };
	_bool	m_bGizmoOp[GizmoEND] = { false, };
};