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
	_float3 viewLightPos = _float3::Transform(CUR_SCENE->Get_LightParams().lights[0].vPosition.xyz(), CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ViewMat());
	_float3 projLightPos = _float3::Transform(viewLightPos, CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ProjMat());
	//[-1,1] -> [0, .5] //[1,-1] -> [0, .5]
	_float2 proj = { projLightPos.x,projLightPos.y };

	
	fontRenderer->Get_Text() = L"x : " + to_wstring(proj.x) + L"/ y : " + to_wstring(proj.y);

	//fontRenderer->Get_Text() = L"X : " + to_wstring(vPos.x) + L"/ Y : " + to_wstring(vPos.y) + L"/ Z : " + to_wstring(vPos.z);

}
