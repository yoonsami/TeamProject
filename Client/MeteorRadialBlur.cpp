#include "pch.h"
#include "MeteorRadialBlur.h"
#include "Camera.h"

void MeteorRadialBlur::Tick()
{
	const _float4x4& matView = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ViewMat();
	const _float4x4& matProj = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ProjMat();

	_float3 vCenterPos = _float3::Transform(Get_Transform()->Get_State(Transform_State::POS).xyz(), matView * matProj);
	CUR_SCENE->g_RadialBlurData.g_vCenterPos = { vCenterPos.x,vCenterPos.y };

	m_fAcc += fDT;

	if (m_fAcc >= m_fExploseTime && m_fAcc <= m_fEndTime)
	{
		CUR_SCENE->g_RadialBlurData.g_bRadialBlurOn = true;
		CUR_SCENE->g_RadialBlurData.g_fNormalRadius = 0.f;
		CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength += 1.f / (m_fEndTime - m_fExploseTime) * fDT;
	}
	else if (m_fAcc > m_fEndTime)
	{
		CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength -= 2.f * fDT;

		if (CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength < 0.f)
		{
			CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength = 0.f;
			CUR_SCENE->g_RadialBlurData.g_bRadialBlurOn = false;
			EVENTMGR.Delete_Object(Get_Owner());
		}

	}
}
