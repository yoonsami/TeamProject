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

PortalScript::PortalScript(SCENE_TYPE eDest)
	: m_eDest(eDest)
{
}

void PortalScript::Tick()
{
	auto pPlayer = CUR_SCENE->Get_Player();
	if (!pPlayer)
		return;

	if(!m_bClicked)
	{
		_float3 vPlayerPos = pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz();
		_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
		_float3 vDir = vPlayerPos - vMyPos;
		vDir.y = 0.f;
		if (vDir.LengthSquared() < 1.f * 1.f)
		{
			// UI Ãâ·Â
			auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
			if (pObj && pObj->Get_Script<UIInteraction>()->Get_Is_Activate(m_pOwner.lock()))
			{
				m_bClicked = true;
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
		m_fAcc += fDT;
		if (m_fAcc >= 2.f)
		{
			m_fAcc = 0.f;

			switch (m_eDest)
			{
			case SCENE_TYPE::GRANSEED:
			{
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<GranseedScene>());
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::FIELD:
			{
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<FieldScene>());
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::MIR:
			{
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<MirScene>());
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::KRIS:
			{
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<KrisScene>());
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
				EVENTMGR.Change_Scene(scene);
				g_bCutScene = false;
			}
				break;
			case SCENE_TYPE::SPIKE:
			{
				shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<SpikeScene>());
				scene->Set_StaticObjects(CUR_SCENE->Get_StaticObjects());
				PHYSX.Set_CharacterControllerNull();
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
