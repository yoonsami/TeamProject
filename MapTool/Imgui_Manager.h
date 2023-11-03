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

	// 맵오브젝트베이스 목록 불러오기
	HRESULT Load_MapObjectBase();
	// 맵오브젝트 생성
	HRESULT Create_MapObject();

private:
// 설치 가능한 오브젝트 목록
	vector<const char*> m_strObjectBaseName;
	_int m_iObjectBaseIndex = { 0 };
// 설치된 오브젝트 목록
	vector<const char*> m_strObjectName;
	_int m_iObjects = { 0 };
	// 현재 설치된 오브젝트들을 가지는 윜포인터
	vector<weak_ptr<GameObject>> m_pMapObjects;
};