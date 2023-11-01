#include "pch.h"
#include "TimeMgr.h"

void TimeMgr::Initialize()
{
	::QueryPerformanceFrequency(&m_liFrequency);
	::QueryPerformanceCounter(&m_liPrevCount);
	m_dTimeScale = 1. / m_liFrequency.QuadPart;
}

void TimeMgr::Tick()
{
	::QueryPerformanceCounter(&m_liCurrCount);

	m_dDeltaTime = static_cast<_double>(m_liCurrCount.QuadPart - m_liPrevCount.QuadPart) * m_dTimeScale;
	m_liPrevCount = m_liCurrCount;
	m_iFrameCount++;
	m_dFrameTime += m_dDeltaTime;

	if (m_dFrameTime > 1.)
	{
		m_iFps = static_cast<_uint>(m_iFrameCount / m_dFrameTime);
		m_dFrameTime = 0.f;
		m_iFrameCount = 0;
	}

	if(m_fSlowDuration > 0.f)
	{
		m_fAcc += _float(m_dDeltaTime);
		if (m_fAcc >= m_fSlowDuration)
		{
			m_fAcc = 0.f;
			m_fSlowDuration = 0.f;
			m_fTimeSpeedRatio = 1.f;
		}
	}
}
