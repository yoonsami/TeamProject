#include "pch.h"
#include "DeleteWhenAnimFinish.h"
#include "ModelAnimator.h"

DeleteWhenAnimFinish::DeleteWhenAnimFinish(const wstring& strAnimName, _float fWaitTime, _bool bAnimLoop, _float fAnimSpeed)
	: m_strAnimName(strAnimName)
	, m_fWaitTime(fWaitTime)
	, m_bAnimLoop(bAnimLoop)
	, m_fAnimSpeed(fAnimSpeed)
{
}

HRESULT DeleteWhenAnimFinish::Init()
{
	if (Get_Owner()->Get_Animator())
	{
		Get_Owner()->Get_Animator()->Set_CurrentAnim(m_strAnimName, m_bAnimLoop, m_fAnimSpeed);
	}
	return S_OK;
}

void DeleteWhenAnimFinish::Tick()
{
	if(!m_bAnimFinished)
	{
		auto animator = Get_Owner()->Get_Animator();
		auto& desc = Get_Owner()->Get_Animator()->Get_TweenDesc();
		if (desc.next.animIndex < 0 && animator->Is_Finished())
			m_bAnimFinished = true;
	}
	else
	{

	}
}
