#include "pch.h"
#include "Feni_FSM.h"
#include "ModelAnimator.h"
#include "Camera.h"
#include "UIInteraction.h"
#include "UiDialogController.h"
#include "ObjectDissolve.h"

HRESULT Feni_FSM::Init()
{
	m_bEntryTeam = true;
    return S_OK;
}

void Feni_FSM::Tick()
{
    State_Tick();

}

void Feni_FSM::State_Tick()
{
	State_Init();
	switch (m_eCurState)
	{
	case Feni_FSM::STATE::SQ_afraid_01:
		SQ_afraid_01();
		break;
	case Feni_FSM::STATE::SQ_afraid_02:
		SQ_afraid_02();
		break;
	case Feni_FSM::STATE::SQ_afraid_03:
		SQ_afraid_03();
		break;
	case Feni_FSM::STATE::SQ_afraid_idle:
		SQ_afraid_idle();
		break;
	case Feni_FSM::STATE::b_idle:
		b_idle();
		break;
	case Feni_FSM::STATE::b_run:
		b_run();
		break;
	case Feni_FSM::STATE::n_idle:
		n_idle();
		break;
	case Feni_FSM::STATE::n_run:
		n_run();
		break;
	case Feni_FSM::STATE::SQ_Surprise:
		SQ_Surprise();
		break;
	case Feni_FSM::STATE::talk:
		talk();
		break;
	case Feni_FSM::STATE::talk_03:
		talk_03();
		break;
	case Feni_FSM::STATE::talk02:
		talk02();
		break;
	}
}

void Feni_FSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Feni_FSM::STATE::SQ_afraid_01:
			SQ_afraid_01_Init();
			break;
		case Feni_FSM::STATE::SQ_afraid_02:
			SQ_afraid_02_Init();
			break;
		case Feni_FSM::STATE::SQ_afraid_03:
			SQ_afraid_03_Init();
			break;
		case Feni_FSM::STATE::SQ_afraid_idle:
			SQ_afraid_idle_Init();
			break;
		case Feni_FSM::STATE::b_idle:
			b_idle_Init();
			break;
		case Feni_FSM::STATE::b_run:
			b_run_Init();
			break;
		case Feni_FSM::STATE::n_idle:
			n_idle_Init();
			break;
		case Feni_FSM::STATE::n_run:
			n_run_Init();
			break;
		case Feni_FSM::STATE::SQ_Surprise:
			SQ_Surprise_Init();
			break;
		case Feni_FSM::STATE::talk:
			talk_Init();
			break;
		case Feni_FSM::STATE::talk_03:
			talk_03_Init();
			break;
		case Feni_FSM::STATE::talk02:
			talk02_Init();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void Feni_FSM::SQ_afraid_01()
{
}

void Feni_FSM::SQ_afraid_01_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"SQ_afraid_01", 0.1f, false, 1.f);
}

void Feni_FSM::SQ_afraid_02()
{
	if (SWITCHMGR.Get_Score(SCORE_TYPE::FENI_QUEST) >= 5)
	{
		if (Get_FinalFrame() - Get_CurFrame() < 5)
			m_eCurState = STATE::SQ_afraid_03;
	}
	else if (Get_FinalFrame() - Get_CurFrame() < 10)
	{
			m_eCurState = STATE::SQ_afraid_idle;
	}
}

void Feni_FSM::SQ_afraid_02_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"SQ_afraid_02", 0.1f, false, 1.f);
}

void Feni_FSM::SQ_afraid_03()
{
	if (Get_FinalFrame() - Get_CurFrame() < 10)
		m_eCurState = STATE::b_idle;
}

void Feni_FSM::SQ_afraid_03_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"SQ_afraid_03", 0.1f, false, 1.f);
}

void Feni_FSM::SQ_afraid_idle()
{
	m_fStateTimer += fDT;
	if (m_fStateTimer >= 3.f)
	{
		m_fStateTimer = 0.f;
		m_eCurState = STATE::SQ_afraid_02;
	}
}

void Feni_FSM::SQ_afraid_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"SQ_afraid_idle", 0.3f, true, 1.f);

	m_fStateTimer = 0.f;
}

void Feni_FSM::b_idle()
{
	if (Can_Interact())
	{
		auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
		if (pObj && pObj->Get_Script<UIInteraction>()->Get_Is_Activate(m_pOwner.lock()))
			m_eCurState = STATE::talk02;
		else if (pObj && !pObj->Get_Script<UIInteraction>()->Is_Created())
			pObj->Get_Script<UIInteraction>()->Create_Interaction(NPCTYPE::FENI, m_pOwner.lock());
	}
	else
	{
		auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
		if (pObj)
			pObj->Get_Script<UIInteraction>()->Remove_Interaction(m_pOwner.lock());
	}
}

void Feni_FSM::b_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_idle", 0.3f, true, 1.f);
}

void Feni_FSM::b_run()
{
	_float3 vDir = m_vGoalPos - Get_Transform()->Get_State(Transform_State::POS).xyz();
	vDir.y = 0;

	if (vDir.LengthSquared() < 3.f * 3.f)
		m_vGoalPos = _float3(120.5f, 0.f, 4.3f);

	vDir.Normalize();
	Soft_Turn_ToInputDir(vDir, XM_PI * 3.f);

	Get_Transform()->Go_Straight();

	if ((_float3(121.f, 0.f, 6.943f) - Get_Transform()->Get_State(Transform_State::POS).xyz()).LengthSquared() < 2.f * 2.f)
	{
		auto script = make_shared<ObjectDissolve>(1.f);
		Get_Owner()->Add_Component(script);
		script->Init();
	}

}

void Feni_FSM::b_run_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run", 0.1f, true, 1.f);
	Get_Transform()->Set_Speed(5.f);
}

void Feni_FSM::n_idle()
{
}

void Feni_FSM::n_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_idle", 0.1f, true, 1.f);
}

void Feni_FSM::n_run()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_run", 0.1f, true, 1.f);
}

void Feni_FSM::n_run_Init()
{
}

void Feni_FSM::SQ_Surprise()
{

}

void Feni_FSM::SQ_Surprise_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"SQ_Surprise", 0.1f, true, 1.f);
}

void Feni_FSM::talk()
{
}

void Feni_FSM::talk_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk", 0.1f, true, 1.f);
}

void Feni_FSM::talk_03()
{
}

void Feni_FSM::talk_03_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk_03", 0.1f, true, 1.f);
}

void Feni_FSM::talk02()
{
	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

	auto pObj = CUR_SCENE->Get_UI(L"UI_NpcDialog_Controller");
	if (pObj && pObj->Get_Script<UiDialogController>()->Get_Dialog_End() == false)
	{
		m_vGoalPos = _float3(121.858f, 0.9f, 34.174f);
		m_eCurState = STATE::b_run;
	}
}

void Feni_FSM::talk02_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk02", 0.1f, true, 1.f);
}

_bool Feni_FSM::Can_Interact()
{

	_float3 pos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	if (_float3::Transform(pos, CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ViewMat()).z < 0)
		return false;

	auto pPlayer = GET_PLAYER;
	if (pPlayer)
	{
		m_pTarget = pPlayer;
		_float lengthSQ = (pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz() - pos).LengthSquared();

		if (lengthSQ < 3.f * 3.f)
			return true;
	}


	return false;
}