#include "pch.h"
#include "DemoAnimationController1.h"
#include "ModelAnimator.h"
#include "ModelAnimation.h"
#include "Model.h"

void DemoAnimationController1::Tick()
{
	if (m_pOwner.expired())
		return;
	auto animator = Get_Owner()->Get_Animator();
	if (!animator)
		return;

	auto model = animator->Get_Model();

	_uint maxAnimCount = animator->Get_Model()->Get_AnimationCount();

	if (KEYTAP(KEY_TYPE::KEY_1))
	{
		m_iCurAnimIndex--;
		if (m_iCurAnimIndex < 0)
			m_iCurAnimIndex += maxAnimCount;

		auto& tweenDesc = animator->Get_TweenDesc();

		tweenDesc.ClearNextAnim();
		tweenDesc.next.animIndex = m_iCurAnimIndex;
		tweenDesc.tweenDuration = 0.2f;

		model->Get_AnimationByIndex(m_iCurAnimIndex)->loop = true;
	}
	if (KEYTAP(KEY_TYPE::KEY_2))
	{
		m_iCurAnimIndex++;
			m_iCurAnimIndex %= maxAnimCount;

		auto& tweenDesc = animator->Get_TweenDesc();

		tweenDesc.ClearNextAnim();
		tweenDesc.next.animIndex = m_iCurAnimIndex;
		tweenDesc.tweenDuration = 0.2f;

		model->Get_AnimationByIndex(m_iCurAnimIndex)->loop = true;
	}

	//if (KEYPUSH(KEY_TYPE::UP))
	//	Get_Transform()->Go_Straight();
}
