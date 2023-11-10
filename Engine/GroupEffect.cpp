#include "pch.h"
#include "GroupEffect.h"

GroupEffect::GroupEffect(shared_ptr<Shader> shader)
	: Component(COMPONENT_TYPE::GroupEffect)
	, m_pShader(shader)
{
}

GroupEffect::~GroupEffect()
{
}

void GroupEffect::Init(void* pArg)
{
	// For. Setting basic info  
	DESC* pDesc = (DESC*)pArg;
	m_tDesc = *pDesc;

	Update_Desc();
}

void GroupEffect::Tick()
{
	if (!m_bStart)
		return;

	_int iNumEndCnt = 0;
	for (auto& iter : m_vEffects)
	{
		if (!iter.second.first)
		{
			if (iter.second.second > m_fCurrAge)
			{
				// TODO 이팩트 생성, 씬에 추가하기 
				iter.second.first = true;
			}
		}
		else
			iter.second.first = false;
	}

	if (iNumEndCnt == m_vEffects.size())
		m_bStart = false;
}

void GroupEffect::Final_Tick()
{
	m_fCurrAge += fDT;
}

void GroupEffect::Update_Desc()
{

}
