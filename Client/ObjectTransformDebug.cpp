#include "pch.h"
#include "ObjectTransformDebug.h"
#include "FontRenderer.h"
#include "MainCameraScript.h"
#include "Model.h"
#include "ModelAnimator.h"
#include "Camera.h"
#include "FSM.h"

void ObjectTransformDebug::Tick()
{
	if (m_pTarget.expired())
	{
		CUR_SCENE->Remove_GameObject(Get_Owner());
		return;
	}

	auto fontRenderer = Get_Owner()->Get_FontRenderer();

	
	//fontRenderer->Get_Text() = L"X : " + to_wstring(vTargetPos.x) + L"/ Y : " + to_wstring(vTargetPos.y) + L"/ Z : " + to_wstring(vTargetPos.z);
	fontRenderer->Get_Text() = L"SpikeHp : " + to_wstring(m_pTarget.lock()->Get_CurHp());

	//fontRenderer->Get_Text() = L"X : " + to_wstring(CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->Get_FixedDist())
								;

}
