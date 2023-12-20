#include "pch.h"
#include "FieldScript.h"
#include <MathUtils.h>
#include "ModelAnimator.h"
#include "Wolf_FSM.h"
#include <OBBBoxCollider.h>
#include "CharacterController.h"
#include "Feni_FSM.h"
#include "Add_Score.h"

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

}
