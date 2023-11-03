#include "pch.h"
#include "ObjectTransformDebug.h"
#include "FontRenderer.h"

void ObjectTransformDebug::Tick()
{
	if (m_pTarget.expired())
	{
		CUR_SCENE->Remove_GameObject(Get_Owner());
		return;
	}

	auto fontRenderer = Get_Owner()->Get_FontRenderer();

	_float3 vPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();

	fontRenderer->Get_Text() = L"X : " + to_wstring(vPos.x) + L"/ Y : " + to_wstring(vPos.y) + L"/ Z : " + to_wstring(vPos.z);

}
