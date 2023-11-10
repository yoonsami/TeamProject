#include "pch.h"
#include "CounterMotionTrailScript.h"
#include "MotionTrailRenderer.h"
#include "ModelAnimator.h"
#include "Model.h"
#include "MotionTrailDisappear.h"

HRESULT CounterMotionTrailScript::Init()
{
	shared_ptr<GameObject> motionTrail = make_shared<GameObject>();
	motionTrail->GetOrAddTransform()->Set_WorldMat(Get_Transform()->Get_WorldMatrix());
	motionTrail->Add_Component(make_shared<MotionTrailRenderer>(RESOURCES.Get<Shader>(L"Shader_Model.fx"), Get_Owner()->Get_Animator()->Get_TweenDesc(), Get_Owner()->Get_Animator()->Get_Model()));
	motionTrail->Add_Component(make_shared<MotionTrailDisappear>(2.f, Color(0.f, 0.f, 1.f, 1.f)));
	motionTrail->Get_Script<MotionTrailDisappear>()->Init();
	CUR_SCENE->Add_GameObject(motionTrail);
	m_pMotionTrail = motionTrail;

	return S_OK;
}

void CounterMotionTrailScript::Tick()
{
	if (!m_pMotionTrail.expired())
	{
		m_tTrailBiggerCool.fAccTime += fDT;

		if (m_tTrailBiggerCool.fAccTime >= m_tTrailBiggerCool.fCoolTime)
		{
			m_fTrailScale += 0.1f;
			m_pMotionTrail.lock()->Get_MotionTrailRenderer()->Set_Scale(m_fTrailScale);
			m_tTrailBiggerCool.fAccTime = 0.f;
		}
	}
	else
	{
		m_fTrailScale = 1.f;
	}
}
