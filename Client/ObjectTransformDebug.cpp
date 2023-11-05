#include "pch.h"
#include "ObjectTransformDebug.h"
#include "FontRenderer.h"
#include "Model.h"
#include "ModelAnimator.h"
#include "FSM.h"

void ObjectTransformDebug::Tick()
{
	if (m_pTarget.expired())
	{
		CUR_SCENE->Remove_GameObject(Get_Owner());
		return;
	}

	auto fontRenderer = Get_Owner()->Get_FontRenderer();

	_float3 vPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	auto& desc = m_pTarget.lock()->Get_Animator()->Get_TweenDesc();
	//fontRenderer->Get_Text() = L"X : " + to_wstring(vPos.x) + L"/ Y : " + to_wstring(vPos.y) + L"/ Z : " + to_wstring(vPos.z);
	//fontRenderer->Get_Text() = L"CurrentAnimation_Frame : " + to_wstring(desc.curr.currentFrame);
	
}
