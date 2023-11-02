#include "pch.h"
#include "DemoFSM.h"
#include "ModelAnimator.h"

HRESULT DemoFSM::Init()
{
    auto animator = Get_Owner()->Get_Animator();
    if (animator)
    {
        // 다음 애니메이션 세팅해주는데, 보간할 예정
        animator->Set_NextTweenAnim(L"b_idle"/*애니메이션 이름*/, 0.2f/*보간 시간*/, true/*반복 애니메이션*/, 1.f/*애니메이션 속도*/);
        m_eCurState = STATE::b_idle;
    }
    return S_OK;
}

void DemoFSM::Tick()
{
    State_Tick();
}

void DemoFSM::State_Tick()
{
	State_Init();

	switch (m_eCurState)
	{
	case STATE::b_idle:
		b_idle();
		break;
	case STATE::b_run_start:
		b_run_start();
		break;
	case STATE::b_run:
		b_run();
		break;
	case STATE::b_run_end_r:
		b_run_end_r();
		break;
	case STATE::b_run_end_l:
		b_run_end_l();
		break;
	}
}

void DemoFSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE::b_idle:
			b_idle_Init();
			break;
		case STATE::b_run_start:
			b_run_start_Init();
			break;
		case STATE::b_run:
			b_run_Init();
			break;
		case STATE::b_run_end_r:
			b_run_end_r_Init();
			break;
		case STATE::b_run_end_l:
			b_run_end_l_Init();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void DemoFSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void DemoFSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void DemoFSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void DemoFSM::b_idle()
{
	
}

void DemoFSM::b_idle_Init()
{
	
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	
	animator->Set_NextTweenAnim(L"b_idle", 0.2f, true, 1.f);
}

void DemoFSM::b_run_start()
{
	_float3 vInputVector = Get_InputDirVector();

	// 방향키를 아무것도 누르지 않으면 상태를 변경
	if (vInputVector == _float3(0.f))
		m_eCurState = STATE::b_idle;
	else
	{	
		// 애니메이션이 끝나면 인데, 보간 시간때문에 어색할 때가 있어서,
		// 바로 이어지는 애니메이션의 경우에는
		// 보간을 안하면서 바꾸거나, 
		// 아래처럼 끝나기 조금 전에 미리 바꾸면 자연스럽게 전환됨
		// 둘 중 선택
		if (Is_AnimFinished())
			m_eCurState = STATE::b_run;

		if (Get_FinalFrame() - Get_CurFrame() < 5)
			m_eCurState = STATE::b_run;

		Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);
	}
}

void DemoFSM::b_run_start_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run_start", 0.2f, false, 1.f);
}

void DemoFSM::b_run()
{
	_float3 vInputVector = Get_InputDirVector();

	// 방향키를 아무것도 누르지 않으면 상태를 변경
	if (vInputVector == _float3(0.f))
		m_eCurState = STATE::b_run_end_r;
}

void DemoFSM::b_run_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);
}

void DemoFSM::b_run_end_r()
{
}

void DemoFSM::b_run_end_r_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run_end_r", 0.2f, false, 1.f);
}

void DemoFSM::b_run_end_l()
{
}

void DemoFSM::b_run_end_l_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run_end_l", 0.2f, false, 1.f);
}
