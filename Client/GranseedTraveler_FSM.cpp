#include "pch.h"
#include "GranseedTraveler_FSM.h"
#include "ModelAnimator.h"

GranseedTraveler_FSM::GranseedTraveler_FSM()
{
}

GranseedTraveler_FSM::~GranseedTraveler_FSM()
{
}

HRESULT GranseedTraveler_FSM::Init()
{
    m_eCurState = STATE::n_idle;
    return S_OK;
}

void GranseedTraveler_FSM::Tick()
{
	State_Tick();
}

void GranseedTraveler_FSM::InteractWithPlayer()
{
	auto pPlayer = CUR_SCENE->Get_GameObject(L"Player");
	m_vDirToTarget = (pPlayer->Get_Transform()->Get_State(Transform_State::POS) - Get_Transform()->Get_State(Transform_State::POS)).xyz();
	m_vDirToTarget.y = 0;
	m_eCurState = STATE::talk_01;
}

void GranseedTraveler_FSM::State_Tick()
{
	State_Init();
	switch (m_eCurState)
	{
	case STATE::n_idle:
		n_idle();
		if (Can_Interact())
			InteractWithPlayer();
		break;

		break;
	case STATE::talk_01:
		talk_01();
		break;

	}
}

void GranseedTraveler_FSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE::n_idle:
			n_idle_Init();
			break;

		case STATE::talk_01:
			talk_Init();
			break;

		}
		m_ePreState = m_eCurState;
	}
}

void GranseedTraveler_FSM::n_idle()
{
}

void GranseedTraveler_FSM::n_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_idle", 0.15f, true, 1.f);
}

void GranseedTraveler_FSM::talk_01()
{
	Look_DirToTarget(XM_PI * 0.5f);

	if (KEYTAP(KEY_TYPE::E)/*talk 종료 조건*/)
		m_eCurState = STATE::n_idle;
}

void GranseedTraveler_FSM::talk_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk_01", 0.15f, true, 1.f);
}

void GranseedTraveler_FSM::Set_State(_uint iIndex)
{
}
