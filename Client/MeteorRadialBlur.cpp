#include "pch.h"
#include "MeteorRadialBlur.h"
#include "Camera.h"
#include "Light.h"

MeteorRadialBlur::MeteorRadialBlur(_float fExploseTime, _float fBiggerSpeed, _float fMaintainTime)
	: m_fExploseTime(fExploseTime), m_fBiggerSpeed(fBiggerSpeed), m_fEndTime(fMaintainTime + fExploseTime)
{
	if (CUR_SCENE->Get_Light())
	{
		auto& lightColor = CUR_SCENE->Get_Light()->Get_Light()->Get_LightInfo().color;
		m_vOriginAmbientColor = lightColor.ambient;
		m_vOriginDiffuseColor = lightColor.diffuse;

		m_vTargetAmbientColor		 = m_vOriginAmbientColor * 0.1f;
		m_vTargetDiffuseColor		 = m_vOriginDiffuseColor * 0.1f;
	}
}

void MeteorRadialBlur::Tick()
{
	const _float4x4& matView = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ViewMat();
	const _float4x4& matProj = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ProjMat();

	_float3 vCenterPos = _float3::Transform(Get_Transform()->Get_State(Transform_State::POS).xyz(), matView * matProj);
	CUR_SCENE->g_RadialBlurData.g_vCenterPos = { vCenterPos.x,vCenterPos.y };

	m_fAcc += fDT;

	auto& lightColor = CUR_SCENE->Get_Light()->Get_Light()->Get_LightInfo().color;
	Color& curAmbientColor = lightColor.ambient;
	Color& curDiffuseColor = lightColor.diffuse;

	_float fTimeOffset = 1.f;

	if (m_fAcc <= m_fExploseTime - fTimeOffset)
	{
		Color targetAmbientColor = m_vTargetAmbientColor;
		Color targetDiffuseColor = m_vTargetDiffuseColor;

		curAmbientColor = _float4::Lerp(m_vOriginAmbientColor, targetAmbientColor, m_fAcc / (m_fExploseTime - fTimeOffset));
		curDiffuseColor = _float4::Lerp(m_vOriginDiffuseColor, targetDiffuseColor, m_fAcc / (m_fExploseTime - fTimeOffset));

	}

	if (m_fAcc >= m_fExploseTime && m_fAcc <= m_fEndTime)
	{
		CUR_SCENE->g_RadialBlurData.g_bRadialBlurOn = true;
		CUR_SCENE->g_RadialBlurData.g_fNormalRadius = 0.f;
		CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength += 1.f / (m_fEndTime - m_fExploseTime) * fDT;
		

	
	}
	else if (m_fAcc > m_fEndTime)
	{
		CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength -= 1.f / (m_fEndTime - m_fExploseTime) * fDT;
		curAmbientColor = _float4::Lerp(curAmbientColor, m_vOriginAmbientColor, 5.f * fDT);
		curDiffuseColor = _float4::Lerp(curDiffuseColor, m_vOriginDiffuseColor, 5.f * fDT);

		if (CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength < 0.f)
		{
			CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength = 0.f;
			CUR_SCENE->g_RadialBlurData.g_bRadialBlurOn = false;
			
			curAmbientColor = m_vOriginAmbientColor;
			curDiffuseColor = m_vOriginDiffuseColor;

			
			EVENTMGR.Delete_Object(Get_Owner());
		}

	}
}
