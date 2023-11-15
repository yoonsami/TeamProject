#include "pch.h"
#include "Model.h"
#include "Gacha_FSM.h"
#include "ModelAnimator.h"

Gacha_FSM::Gacha_FSM(const Gacha_FSM_Desc& desc)
    : m_Desc(desc)
{
}

HRESULT Gacha_FSM::Init()
{
	auto animator = Get_Owner()->Get_Animator();
	if (animator)
	{
		animator->Set_CurrentAnim(m_Desc.strAnimTag, true, 1.f);
		m_eCurState = STATE::SQ_SpecialHero;
	}

	m_iDummy_CP_BoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_CP");
	m_iCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Cam");
	m_iSkillCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_SkillCam");


	m_pCamera = CUR_SCENE->Get_MainCamera();

	return S_OK;
}

void Gacha_FSM::Tick()
{
	State_Tick();
}

void Gacha_FSM::State_Tick()
{
	State_Init();

	switch (m_eCurState)
	{
	case Gacha_FSM::STATE::SQ_SpecialHero:
		SQ_SpecialHero();
		break;
	case Gacha_FSM::STATE::NONE:
		break;

	}
}

void Gacha_FSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Gacha_FSM::STATE::SQ_SpecialHero:
			SQ_SpecialHero_Init();
			break;
		case Gacha_FSM::STATE::NONE:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void Gacha_FSM::SQ_SpecialHero()
{
	if (Get_FinalFrame() - Get_CurFrame() < 10)
	{
		shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

		animator->Set_AnimationSpeed(0.3f);
	}
}

void Gacha_FSM::SQ_SpecialHero_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_CurrentAnim(m_Desc.strAnimTag, false, 1.f);
}
