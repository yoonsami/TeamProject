#include "pch.h"
#include "FieldScript.h"
#include <MathUtils.h>
#include "ModelAnimator.h"
#include "Wolf_FSM.h"
#include <OBBBoxCollider.h>
#include "CharacterController.h"
#include "Feni_FSM.h"
#include "Add_Score.h"
#include "MainCameraScript.h"
#include "FisherMan_FSM.h"
#include "UiMessageCreater.h"
#include "Silversword_Soldier_FSM.h"
#include "Succubus_Scythe_FSM.h"
#include "Undead_Priest_FSM.h"
#include "Combat5_LightControl.h"

FieldScript::FieldScript(shared_ptr<GameObject> pPlayer, _float fDetectRange)
	: m_pPlayer(pPlayer)
	, m_fDetectRange(fDetectRange)
{

}

void FieldScript::Tick()
{
	_float3 vPlayerPos = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	if ((vPlayerPos - vMyPos).LengthSquared() < m_fDetectRange * m_fDetectRange)
	{
		if (SWITCHMGR.Get_SwitchState(SWITCH_TYPE::CREATE_WOLF_AFTER_DELLONS))
		{
			SWITCHMGR.Set_SwitchState(SWITCH_TYPE::CREATE_WOLF_AFTER_DELLONS, false);

			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
			for (_uint i = 0; i < 5; ++i)
			{
				_float3 vRandVector = MathUtils::Get_RandomVector(_float3(-1.f, 0.f, -1.f), _float3(1.f, 0.f, 1.f));
				_float vRandRange = MathUtils::Get_RandomFloat(2.f, 5.f);

				_float3 vPos = _float3(87.5f, 10.f, 31.f) + vRandRange * vRandVector;

				shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();
				ObjMonster->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(vPos, 1.f));

				shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
				{
					shared_ptr<Model> model = RESOURCES.Get<Model>(L"Wolf");
					animator->Set_Model(model);
				}
				ObjMonster->Add_Component(animator);
				ObjMonster->Add_Component(make_shared<Wolf_FSM>());
				ObjMonster->Get_FSM()->Set_Target(m_pPlayer.lock());
				ObjMonster->Get_FSM()->Init();
				ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.7f, 0.5f })); //obbcollider
				ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
				ObjMonster->Get_Collider()->Set_Activate(true);
				ObjMonster->Add_Component(make_shared<Add_Score>(SCORE_TYPE::FENI_QUEST));
				{
					auto controller = make_shared<CharacterController>();
					ObjMonster->Add_Component(controller);
					auto& desc = controller->Get_CapsuleControllerDesc();
					desc.radius = 0.5f;
					desc.height = 5.f;
					desc.position = { vPos.x, vPos.y, vPos.z };
					controller->Create_Controller();
					ObjMonster->Set_ObjectGroup(OBJ_MONSTER);
				}

				EVENTMGR.Create_Object(ObjMonster);
			}
			{
				_float3 vPos = _float3(87.5f, 10.f, 31.f);
				SOUND.Play_Sound(L"EDa_ChangChunVillager02_I05_1", SOUND_EFFECT, 0.5f, vPos,15.f);
				shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();
				ObjMonster->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(vPos, 1.f));

				shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
				{
					shared_ptr<Model> model = RESOURCES.Get<Model>(L"Feni");
					animator->Set_Model(model);
				}
				ObjMonster->Add_Component(animator);
				ObjMonster->Add_Component(make_shared<Feni_FSM>());
				ObjMonster->Get_FSM()->Set_Target(m_pPlayer.lock());
				ObjMonster->Get_FSM()->Init();
				{
					auto controller = make_shared<CharacterController>();
					ObjMonster->Add_Component(controller);
					auto& desc = controller->Get_CapsuleControllerDesc();
					desc.radius = 0.5f;
					desc.height = 5.f;
					desc.position = { vPos.x, vPos.y, vPos.z };
					controller->Create_Controller();
					ObjMonster->Set_ObjectGroup(OBJ_TEAM);
				}

				EVENTMGR.Create_Object(ObjMonster);
			}



		}

	}


	if (SWITCHMGR.Get_SwitchState(SWITCH_TYPE::CREATE_COMBAT4_AFTER_SPIKE) && SWITCHMGR.Get_Score(SCORE_TYPE::TENT_RESCUE) >= 11)
	{
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::CREATE_COMBAT4_AFTER_SPIKE, false);
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::AFTER_COMBAT4_PEOPLE_COMBACK, true);
		
	}
	if (SWITCHMGR.Get_SwitchState(SWITCH_TYPE::AFTER_COMBAT4_PEOPLE_COMBACK))
	{
		m_fAcc += fDT;
		if (m_fAcc >= 2.f)
		{
			SWITCHMGR.Set_SwitchState(SWITCH_TYPE::AFTER_COMBAT4_PEOPLE_COMBACK, false);
			{
				auto camera = CUR_SCENE->Get_MainCamera();
				auto script = camera->Get_Script<MainCameraScript>();
				script->Fix_Camera(5.f, _float3(0.f), 5.f);
				_float3 vLookPos = _float3(89.512f, -12.5f, 120.25f);
				_float3 vDir = _float3(88.580f, 1.656f, 134.472f) - vLookPos;
				_float fDist = vDir.Length();
				vDir.Normalize();
				script->Set_FixedDir(vDir);
				script->Set_FixedDist(fDist);
				script->Set_FixedLookTarget(vLookPos);

				auto pObj = CUR_SCENE->Get_UI(L"UI_Message_Controller");
				if (pObj)
					pObj->Get_Script<UiMessageCreater>()->Create_Message(L"사람들이 돌아온다");
			}

			{
				_float3 vPos = { 112.f,-10.5f,116.f };
				_float3 npcLook = { -1.f,0.f,0.f };

				shared_ptr<GameObject> npc = make_shared<GameObject>();
				npc->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(vPos, 1.f));
				npc->GetOrAddTransform()->Set_LookDir(npcLook);

				shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
				shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
				{
					shared_ptr<Model> model = RESOURCES.Get<Model>(L"FisherMan1");
					animator->Set_Model(model);
				}
				npc->Add_Component(animator);
				npc->Add_Component(make_shared<FisherMan_FSM>(_float3(67.f, -12.5f, 113.5f)));
				npc->Get_FSM()->Set_Target(m_pPlayer.lock());
				npc->Get_FSM()->Init();

				{
					auto controller = make_shared<CharacterController>();
					npc->Add_Component(controller);
					auto& desc = controller->Get_CapsuleControllerDesc();
					desc.radius = 0.5f;
					desc.height = 5.f;
					desc.position = { vPos.x, vPos.y, vPos.z };
					controller->Create_Controller();
					npc->Set_ObjectGroup(OBJ_MONSTER);
				}
				npc->Set_DrawShadow(true);
				EVENTMGR.Create_Object(npc);
			}
			{
				_float3 vPos = { 102.f,-10.5f,126.f };
				_float3 npcLook = { 1.f,0.f,-1.f };

				shared_ptr<GameObject> npc = make_shared<GameObject>();
				npc->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(vPos, 1.f));
				npc->GetOrAddTransform()->Set_LookDir(npcLook);

				shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
				shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
				{
					shared_ptr<Model> model = RESOURCES.Get<Model>(L"FisherMan2");
					animator->Set_Model(model);
				}
				npc->Add_Component(animator);
				npc->Add_Component(make_shared<FisherMan_FSM>(_float3(95.f, -12.5f, 123.f)));
				npc->Get_FSM()->Set_Target(m_pPlayer.lock());
				npc->Get_FSM()->Init();

				{
					auto controller = make_shared<CharacterController>();
					npc->Add_Component(controller);
					auto& desc = controller->Get_CapsuleControllerDesc();
					desc.radius = 0.5f;
					desc.height = 5.f;
					desc.position = { vPos.x, vPos.y, vPos.z };
					controller->Create_Controller();
					npc->Set_ObjectGroup(OBJ_MONSTER);
				}
				npc->Set_DrawShadow(true);
				EVENTMGR.Create_Object(npc);
			}
			{
				_float3 vPos = { 112.f,-10.5f,121.f };
				_float3 npcLook = { 1.f,0.f,-1.f };

				shared_ptr<GameObject> npc = make_shared<GameObject>();
				npc->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(vPos, 1.f));
				npc->GetOrAddTransform()->Set_LookDir(npcLook);

				shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
				shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
				{
					shared_ptr<Model> model = RESOURCES.Get<Model>(L"FisherMan1");
					animator->Set_Model(model);
				}
				npc->Add_Component(animator);
				npc->Add_Component(make_shared<FisherMan_FSM>(_float3(104.f, -12.5f, 114.f)));
				npc->Get_FSM()->Set_Target(m_pPlayer.lock());
				npc->Get_FSM()->Init();

				{
					auto controller = make_shared<CharacterController>();
					npc->Add_Component(controller);
					auto& desc = controller->Get_CapsuleControllerDesc();
					desc.radius = 0.5f;
					desc.height = 5.f;
					desc.position = { vPos.x, vPos.y, vPos.z };
					controller->Create_Controller();
					npc->Set_ObjectGroup(OBJ_MONSTER);
				}
				npc->Set_DrawShadow(true);
				EVENTMGR.Create_Object(npc);
			}
		}
	}

	if (SWITCHMGR.Get_SwitchState(SWITCH_TYPE::DELLONS_DIALOG) && SWITCHMGR.Get_SwitchState(SWITCH_TYPE::SPIKE_DIALOG))
	{
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::DELLONS_DIALOG, false);
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::SPIKE_DIALOG, false);
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::CREATE_COMBAT5_AFTER_SPIKE, false);
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::COMBAT5_CREATED, true);

		Load_Monster(20, L"Silversword_Soldier", m_pPlayer.lock());
		Load_Monster(20, L"Succubus_Scythe", m_pPlayer.lock());
		Load_Monster(20, L"Undead_Priest", m_pPlayer.lock());

		shared_ptr<GameObject> lightController = make_shared<GameObject>();
		lightController->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(155.f,-1.3f,60.f,1.f));
		lightController->Add_Component(make_shared<Combat5_LightControl>(m_pPlayer.lock()));

		EVENTMGR.Create_Object(lightController);
	}
}

void FieldScript::Load_Monster(_uint iCnt, const wstring& strMonsterTag, shared_ptr<GameObject> pPlayer)
{
	for (_uint i = 0; i < iCnt; i++)
	{
		// Add. Monster
		shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

		ObjMonster->Add_Component(make_shared<Transform>());

		_float3 vSpawnPos = MathUtils::Get_RandomVector(_float3{ 165.f, 1.f, 50.f }, _float3{ 235.f, 1.f, 60.f });
		ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4{ vSpawnPos.x, vSpawnPos.y, vSpawnPos.z, 1.f });
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(strMonsterTag);
				animator->Set_Model(model);
			}

			ObjMonster->Add_Component(animator);

			if (strMonsterTag == L"Silversword_Soldier")
				ObjMonster->Add_Component(make_shared<Silversword_Soldier_FSM>());
			else if (strMonsterTag == L"Succubus_Scythe")
				ObjMonster->Add_Component(make_shared<Succubus_Scythe_FSM>());
			else if (strMonsterTag == L"Undead_Priest")
				ObjMonster->Add_Component(make_shared<Undead_Priest_FSM>());

			ObjMonster->Get_FSM()->Set_Target(pPlayer);
		}
		ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.7f, 0.5f })); //obbcollider
		ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
		ObjMonster->Get_Collider()->Set_Activate(true);

		wstring strMonsterName = strMonsterTag + to_wstring(i);
		ObjMonster->Set_Name(strMonsterName);
		{
			//Alpaca and Wolf don't use character controller

				auto controller = make_shared<CharacterController>();
				ObjMonster->Add_Component(controller);
				auto& desc = controller->Get_CapsuleControllerDesc();
				desc.radius = 0.5f;
				desc.height = 5.f;
				_float3 vPos = ObjMonster->Get_Transform()->Get_State(Transform_State::POS).xyz();
				desc.position = { vPos.x, vPos.y, vPos.z };
				controller->Create_Controller();
			
		}
		ObjMonster->Set_ObjectGroup(OBJ_MONSTER);
		ObjMonster->Get_FSM()->Init();
		ObjMonster->Set_DrawShadow(true);
		EVENTMGR.Create_Object(ObjMonster);
	}
}
