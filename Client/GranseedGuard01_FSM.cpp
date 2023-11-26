#include "pch.h"
#include "GranseedGuard01_FSM.h"
#include "ModelAnimator.h"

GranseedGuard01_FSM::GranseedGuard01_FSM()
{
}

GranseedGuard01_FSM::~GranseedGuard01_FSM()
{
}

HRESULT GranseedGuard01_FSM::Init()
{
    m_eCurState = STATE::n_idle;
    return S_OK;
}

void GranseedGuard01_FSM::Tick()
{
    State_Tick();
}

void GranseedGuard01_FSM::State_Tick()
{
	switch (m_eCurState)
	{
	case GranseedGuard01_FSM::STATE::n_idle:
		n_idle();
		break;
	case GranseedGuard01_FSM::STATE::run:
		run();
		break;
	case GranseedGuard01_FSM::STATE::talk:
		talk();
		break;
	case GranseedGuard01_FSM::STATE::walk:
		walk();
		break;
	}
}

void GranseedGuard01_FSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case GranseedGuard01_FSM::STATE::n_idle:
			n_idle_Init();
			break;
		case GranseedGuard01_FSM::STATE::run:
			run_Init();
			break;
		case GranseedGuard01_FSM::STATE::talk:
			talk_Init();
			break;
		case GranseedGuard01_FSM::STATE::walk:
			walk_Init();
			break;
		}
	}
}

void GranseedGuard01_FSM::n_idle()
{
	m_fStateAcc += fDT;
	if (m_fStateAcc >= m_fIdleTimer)
	{
		_float4x4 matWorld = Get_Transform()->Get_WorldMatrix();
		
		_float3 rayPos = matWorld.Translation() + matWorld.Up();
		_float3 rayDir = matWorld.Backward();

		physx::PxRaycastBuffer hit{};
		physx::PxQueryFilterData filterData;
		filterData.flags = physx::PxQueryFlag::eSTATIC;
		if (PHYSX.Get_PxScene()->raycast({ rayPos.x,rayPos.y,rayPos.z }, { rayDir.x,rayDir.y,rayDir.z }, 2.f, hit, PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData))
		{
			if (hit.getAnyHit(0).distance < 2.f)
			{
				//Turn;
			}
		}
		else
		{

		}
	}
}

void GranseedGuard01_FSM::n_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"n_idle", 0.15f, true, 1.f);

}

void GranseedGuard01_FSM::run()
{
	m_fStateAcc += fDT;
	if (m_fStateAcc >= m_fMoveTimer)
	{
		m_fStateAcc = 0.f;
		m_eCurState = STATE::n_idle;
	}
}

void GranseedGuard01_FSM::run_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"run", 0.15f, true, 1.f);
}

void GranseedGuard01_FSM::talk()
{
	m_fStateAcc += fDT;
	if (m_fStateAcc >= m_fMoveTimer)
	{
		m_fStateAcc = 0.f;
		m_eCurState = STATE::n_idle;
	}
}

void GranseedGuard01_FSM::talk_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"talk", 0.15f, false, 1.f);
}

void GranseedGuard01_FSM::walk()
{
	m_fStateAcc += fDT;
	if (m_fStateAcc >= m_fMoveTimer)
	{
		m_fStateAcc = 0.f;
		m_eCurState = STATE::n_idle;
	}

}

void GranseedGuard01_FSM::walk_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"walk", 0.15f, true, 1.f);
}

void GranseedGuard01_FSM::Set_State(_uint iIndex)
{
}
