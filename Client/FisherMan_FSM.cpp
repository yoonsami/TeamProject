#include "pch.h"
#include "FisherMan_FSM.h"
#include "ModelAnimator.h"
#include "Camera.h"
#include "UIInteraction.h"
#include "UiDialogController.h"
#include "ObjectDissolve.h"

HRESULT FisherMan_FSM::Init()
{
	m_eCurState = STATE::n_walk;
    return S_OK;
}

void FisherMan_FSM::Tick()
{
    State_Tick();

}

void FisherMan_FSM::State_Tick()
{
	State_Init();
	switch (m_eCurState)
	{
	case FisherMan_FSM::STATE::n_idle:
		n_idle();
		break;
	case FisherMan_FSM::STATE::n_walk:
		n_walk();
		break;
	case FisherMan_FSM::STATE::talk_01:
		talk_01();
		break;
	}
}

void FisherMan_FSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case FisherMan_FSM::STATE::n_idle:
			n_idle_Init();
			break;
		case FisherMan_FSM::STATE::n_walk:
			n_walk_Init();
			break;
		case FisherMan_FSM::STATE::talk_01:
			talk_01_Init();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void FisherMan_FSM::n_idle()
{
	if (Can_Interact())
	{
		auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
		if (pObj && pObj->Get_Script<UIInteraction>()->Get_Is_Activate(m_pOwner.lock()))
			m_eCurState = STATE::talk_01;
		else if (pObj && !pObj->Get_Script<UIInteraction>()->Is_Created())
			pObj->Get_Script<UIInteraction>()->Create_Interaction(NPCTYPE::FISHERMAN, m_pOwner.lock());
	}
	else
	{
		auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
		if (pObj)
			pObj->Get_Script<UIInteraction>()->Remove_Interaction(m_pOwner.lock());
	}
}

void FisherMan_FSM::n_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_idle", 0.1f, true, 1.f);
}

void FisherMan_FSM::n_walk()
{
	_float3 vDir = m_vGoalPos - Get_Transform()->Get_State(Transform_State::POS).xyz();
	vDir.y = 0;

	if (vDir.LengthSquared() < 2.f * 2.f)
		m_eCurState = STATE::n_idle;

	vDir.Normalize();
	Soft_Turn_ToInputDir(vDir, XM_PI);

	Get_Transform()->Go_Straight();

}

void FisherMan_FSM::n_walk_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_walk", 0.1f, true, 1.f);

	Get_Transform()->Set_Speed(2.f);
}

void FisherMan_FSM::talk_01()
{
	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

	auto pObj = CUR_SCENE->Get_UI(L"UI_NpcDialog_Controller");
	if (pObj && pObj->Get_Script<UiDialogController>()->Get_Dialog_End() == false)
	{
		m_eCurState = STATE::n_idle;
	}
}


void FisherMan_FSM::talk_01_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk_01", 0.1f, true, 1.f);
}

_bool FisherMan_FSM::Can_Interact()
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