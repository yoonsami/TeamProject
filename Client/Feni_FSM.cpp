#include "pch.h"
#include "Feni_FSM.h"
#include "ModelAnimator.h"

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
}

void Feni_FSM::b_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_idle", 0.3f, true, 1.f);
}

void Feni_FSM::b_run()
{
}

void Feni_FSM::b_run_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run", 0.1f, true, 1.f);
}

void Feni_FSM::n_idle()
{
}

void Feni_FSM::n_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_idle_Init", 0.1f, true, 1.f);
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
}

void Feni_FSM::talk02_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk02", 0.1f, true, 1.f);
}
