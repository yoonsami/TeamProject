#pragma once

#include "pch.h"

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
	// 선택한 오브젝트의 정보.
	void Frame_ObjectDesc();
	// 방향성광원의 정보 관리.
	void Frame_DirectionalLight();
	// 피킹
	void Picking_Object();

	// 맵오브젝트베이스 목록 불러오기
	HRESULT Load_MapObjectBase();
	// 맵오브젝트 생성
	HRESULT Create_MapObject();
	// 맵오브젝트 제거
	HRESULT Delete_MapObject();
	// 맵오브젝트 저장
	HRESULT Save_MapObject();
	// 맵오브젝트 불러오기
	HRESULT Load_MapObject();

private:
// 설치 가능한 오브젝트 목록
	vector<const char*> m_strObjectBaseNameList;
	// 같은이름의 모델의 중복개수(모델별 번호)
	vector<_uint> m_iObjectBaseIndexList;
	vector<shared_ptr<char[]>> m_strObjectBaseNamePtr;
	_int m_iObjectBaseIndex = { 0 };
	// 생성하는 오브젝트에 넣고싶은 UVWeight - UV에 곱해지는 숫자
	_float m_fUVWeight = { 1.f };
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
// 기즈모관련
	GizmoOp m_eGizmoOp = { GizmoTR };
	_bool	m_bGizmoOp[GizmoEND] = { false, };
};