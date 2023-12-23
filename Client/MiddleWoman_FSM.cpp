#include "pch.h"
#include "MiddleWoman_FSM.h"
#include "ModelAnimator.h"
#include "Camera.h"
#include "UIInteraction.h"
#include "UiDialogController.h"
#include "ObjectDissolve.h"
#include "UiQuestController.h"

HRESULT MiddleWoman_FSM::Init()
{
	m_eCurState = STATE::n_idle;
    return S_OK;
}

void MiddleWoman_FSM::Tick()
{
    State_Tick();

}

void MiddleWoman_FSM::State_Tick()
{
	State_Init();
	switch (m_eCurState)
	{
	case MiddleWoman_FSM::STATE::n_idle:
		n_idle();
		break;

	case MiddleWoman_FSM::STATE::talk:
		talk();
		break;
	}
}

void MiddleWoman_FSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case MiddleWoman_FSM::STATE::n_idle:
			n_idle_Init();
			break;

		case MiddleWoman_FSM::STATE::talk:
			talk_Init();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void MiddleWoman_FSM::n_idle()
{
	if (Can_Interact())
	{
		auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
		if (pObj && pObj->Get_Script<UIInteraction>()->Get_Is_Activate(m_pOwner.lock()))
			m_eCurState = STATE::talk;
		else if (pObj && !pObj->Get_Script<UIInteraction>()->Is_Created())
			pObj->Get_Script<UIInteraction>()->Create_Interaction(NPCTYPE::QUEST, m_pOwner.lock());
	}
	else
	{
		auto pObj = CUR_SCENE->Get_UI(L"UI_Interaction");
		if (pObj)
			pObj->Get_Script<UIInteraction>()->Remove_Interaction(m_pOwner.lock());
	}
}

void MiddleWoman_FSM::n_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_idle", 0.1f, true, 1.f);
}

void MiddleWoman_FSM::talk()
{
	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

	auto obj = CUR_SCENE->Get_UI(L"UI_Dialog_Controller");
	if (obj && !obj->Get_Script<UiQuestController>()->Get_Dialog_End())
	{
		if (obj->Get_Script<UiQuestController>()->Get_CurState(QUESTINDEX::KILL_DELLONS) == CUR_QUEST::PROGRESS)
		m_eCurState = STATE::n_idle;
	}
}


void MiddleWoman_FSM::talk_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk", 0.1f, true, 1.f);
}

_bool MiddleWoman_FSM::Can_Interact()
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