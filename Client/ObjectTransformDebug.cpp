#include "pch.h"
#include "ObjectTransformDebug.h"
#include "FontRenderer.h"
#include "MainCameraScript.h"

void ObjectTransformDebug::Tick()
{
	if (m_pTarget.expired())
	{
		CUR_SCENE->Remove_GameObject(Get_Owner());
		return;
	}

	auto fontRenderer = Get_Owner()->Get_FontRenderer();

	_float3 vPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	vPos += _float3::Up;
	_float3 vCameraPos = CUR_SCENE->Get_MainCamera()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	
	_float3 vOffset = CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->Get_Offset();
	
	fontRenderer->Get_Text() = L"Length : " + to_wstring((vPos - vCameraPos).Length()) + L"/ OffsetLength : " + to_wstring(vOffset.Length());

	//fontRenderer->Get_Text() = L"X : " + to_wstring(vPos.x) + L"/ Y : " + to_wstring(vPos.y) + L"/ Z : " + to_wstring(vPos.z);

}
