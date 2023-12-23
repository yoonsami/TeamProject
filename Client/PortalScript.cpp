#include "pch.h"
#include "PortalScript.h"
#include "UIInteraction.h"
#include "LoadingScene.h"
#include "GranseedScene.h"
#include "FieldScene.h"
#include "MirScene.h"
#include "KrisScene.h"
#include "SpikeScene.h"
#include "GroupEffect.h"
#include "EffectGoUp.h"
#include "MainCameraScript.h"
#include "MainUiController.h"

PortalScript::PortalScript(SCENE_TYPE eDest, const _float3& vPos)
	: m_eDest(eDest)
	, m_vStartingPos(vPos)
{
}

void PortalScript::Tick()
{
	auto pPlayer = CUR_SCENE->Get_Player();
	if (!pPlayer)
		return;
	_float3 vPlayerPos = pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz();
	if(!m_bClicked)
	{
		
		_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
		_float3 vDir = vPlayerPos - vMyPos;
		vDir.y = 0.f;
		if (vDir.LengthSquared() < 1.f * 1.f)
		{
			// UI 출력 
			auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
			if (pObj && pObj->Get_Script<UIInteraction>()->Get_Is_Activate(m_pOwner.lock()))
			{
				auto pController = CUR_SCENE->Get_UI(L"Main_UI_Controller");
				if (pController)
					pController->Get_Script<MainUiController>()->Set_MainUI_Render(false);

				m_bClicked = true;
				// 사운드재생
				SOUND.Play_Sound(L"SQ_TeleportOut_Rune_01", CHANNELID::SOUND_EFFECT, g_fEnvironmentRatio, m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz(), 10.f/*MagicNumber*/);
				{
					wstring strSkilltag = L"Portal_Effect";
					shared_ptr<GameObject> pGroupEffectObj = make_shared<GameObject>();

					// For. Transform 
					pGroupEffectObj->GetOrAddTransform();
					pGroupEffectObj->Get_Transform()->Set_State(Transform_State::POS, m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS));
					pGroupEffectObj->Set_Name(strSkilltag);
					// For. GroupEffectData 
					wstring wstrFileName = strSkilltag + L".dat";
					wstring wtsrFilePath = TEXT("..\\Resources\\EffectData\\GroupEffectData\\") + wstrFileName;
					shared_ptr<GroupEffectData> pGroupEffectData = RESOURCES.GetOrAddGroupEffectData(strSkilltag, wtsrFilePath);

					if (pGroupEffectData == nullptr)
						return;

					// For. GroupEffect component 
					shared_ptr<GroupEffect> pGroupEffect = make_shared<GroupEffect>();
					pGroupEffectObj->Add_Component(pGroupEffect);
					pGroupEffectObj->Get_GroupEffect()->Set_Tag(pGroupEffectData->Get_GroupEffectDataTag());
					pGroupEffectObj->Get_GroupEffect()->Set_MemberEffectData(pGroupEffectData->Get_MemberEffectData());
					pGroupEffectObj->Get_GroupEffect()->Set_InitWorldMatrix(Get_Transform()->Get_WorldMatrix());
					pGroupEffectObj->Get_GroupEffect()->Set_MemberEffectMaterials();
					pGroupEffectObj->Set_Name(strSkilltag);
					pGroupEffectObj->Init();
					m_pEffectObj = pGroupEffectObj;
					EVENTMGR.Create_Object(pGroupEffectObj);
				}

				{
					if (CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>())
						CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(vPlayerPos + _float3::Up);

				}

			}
			else if (pObj && !pObj->Get_Script<UIInteraction>()->Is_Created())
				pObj->Get_Script<UIInteraction>()->Create_Interaction(NPCTYPE::PORTAL, m_pOwner.lock());


		}
		else
		{
			auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
			if (pObj)
				pObj->Get_Script<UIInteraction>()->Remove_Interaction(m_pOwner.lock());

		}
	}
	else
	{
		if (CUR_SCENE->Get_MainCamera())
		{
			auto script = CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>();
			_float3 vCurTargetPos = script->Get_FixedPos();
			_float3 vTargetPos = vPlayerPos + _float3::Up * 2.f;

			
			_float3 vTargetDir = -pPlayer->Get_Transform()->Get_State(Transform_State::LOOK).xyz();
			vTargetDir.y = 0.f;

			vTargetDir.Normalize();

			vTargetPos = _float3::Lerp(vCurTargetPos, vTargetPos, 3.f * fDT);
			_float fDist = script->Get_FixedDist();
			fDist = _float2::Lerp(_float2(fDist), _float2(0.01f), 3.f*fDT).x;
			script->Fix_Camera(0.1f, vTargetDir, fDist);
			script->Set_FixedLookTarget(vTargetPos);

		}
		CUR_SCENE->g_fBrightness += 0.2f * fDT;
		m_fAcc += fDT;
		if (m_fAcc >= 5.f)
		{
			m_fAcc = 0.f;
			m_bClicked = false;
			CUR_SCENE->g_fBrightness = 0.f;
			switch (m_eDest)
			{
			case SCENE_TYPE::GRANSEED:
			{
				auto nextScene = make_shared<GranseedScene>();
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(nextScene);
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				nextScene->Set_StartingPos(m_vStartingPos);
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::FIELD:
			{
				auto nextScene = make_shared<FieldScene>();
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(nextScene);
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				nextScene->Set_StartingPos(m_vStartingPos);
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::MIR:
			{
				auto nextScene = make_shared<MirScene>();
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(nextScene);
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				nextScene->Set_StartingPos(m_vStartingPos);
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::KRIS:
			{
				auto nextScene = make_shared<KrisScene>();
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(nextScene);
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				nextScene->Set_StartingPos(m_vStartingPos);
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::SPIKE:
			{
				auto nextScene = make_shared<SpikeScene>();
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(nextScene);
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				nextScene->Set_StartingPos(m_vStartingPos);
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			default:
				break;
			}


		}

		
	}

}
