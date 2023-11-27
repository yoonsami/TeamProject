#include "pch.h"
#include "GranseedChildren_FSM.h"
#include "ModelAnimator.h"

GranseedChildren_FSM::GranseedChildren_FSM()
{
}

GranseedChildren_FSM::~GranseedChildren_FSM()
{
}

HRESULT GranseedChildren_FSM::Init()
{
    m_eCurState = STATE::n_idle;
    return S_OK;
}

void GranseedChildren_FSM::Tick()
{
	State_Tick();
}

void GranseedChildren_FSM::InteractWithPlayer()
{
	auto pPlayer = CUR_SCENE->Get_GameObject(L"Player");
	m_vDirToTarget = (pPlayer->Get_Transform()->Get_State(Transform_State::POS) - Get_Transform()->Get_State(Transform_State::POS)).xyz();
	m_vDirToTarget.y = 0;
	m_eCurState = STATE::talk_01;
}

void GranseedChildren_FSM::State_Tick()
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

void GranseedChildren_FSM::State_Init()
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

void GranseedChildren_FSM::n_idle()
{
}

void GranseedChildren_FSM::n_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_idle", 0.15f, true, 1.f);
}

void GranseedChildren_FSM::talk_01()
{
	Look_DirToTarget(XM_PI * 0.5f);

	if (KEYTAP(KEY_TYPE::E)/*talk 종료 조건*/)
		m_eCurState = STATE::n_idle;
}

void GranseedChildren_FSM::talk_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk_01", 0.15f, true, 1.f);
}

void GranseedChildren_FSM::n_run()
{
}

void GranseedChildren_FSM::n_run_Init()
{
}

void GranseedChildren_FSM::n_walk()
{
}

void GranseedChildren_FSM::n_walk_Init()
{
}

void GranseedChildren_FSM::Set_State(_uint iIndex)
{
}
