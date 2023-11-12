#include "pch.h"
#include "EffectMovementScript.h"

#include "Utils.h"

void EffectMovementScript::Init(void* pArg)
{
	// For. Setting basic info  
	MeshEffectData::Transform_Desc* pDesc = (MeshEffectData::Transform_Desc*)pArg;

	// For. Initial Transform 
	m_vStartPos = _float3(
		GetRandomFloatInRange(pDesc->vPosRange.x / 2.f * (-1.f), pDesc->vPosRange.x / 2.f),
		GetRandomFloatInRange(pDesc->vPosRange.y / 2.f * (-1.f), pDesc->vPosRange.y / 2.f),
		GetRandomFloatInRange(pDesc->vPosRange.z / 2.f * (-1.f), pDesc->vPosRange.z / 2.f)
	);

	m_vStartScale = _float3(
		GetRandomFloatInRange(pDesc->vInitScale_Min.x, pDesc->vInitScale_Max.x),
		GetRandomFloatInRange(pDesc->vInitScale_Min.y, pDesc->vInitScale_Max.y),
		GetRandomFloatInRange(pDesc->vInitScale_Min.z, pDesc->vInitScale_Max.z)
	);

	m_vStartRotation = _float3(
		GetRandomFloatInRange(pDesc->vInitRotation_Min.x, pDesc->vInitRotation_Max.x),
		GetRandomFloatInRange(pDesc->vInitRotation_Min.y, pDesc->vInitRotation_Max.y),
		GetRandomFloatInRange(pDesc->vInitRotation_Min.z, pDesc->vInitRotation_Max.z)
	);

	// For. Translate
	m_iTranslateOption = pDesc->iTranslateOption;
	m_fTranslateSpeed = pDesc->fTranslateSpeed;
	m_vEndPos = _float3(
		GetRandomFloatInRange(pDesc->vEndPosOffset_Min.x, pDesc->vEndPosOffset_Max.x),
		GetRandomFloatInRange(pDesc->vEndPosOffset_Min.y, pDesc->vEndPosOffset_Max.y),
		GetRandomFloatInRange(pDesc->vEndPosOffset_Min.z, pDesc->vEndPosOffset_Max.z)
	);

	m_vEndScale = pDesc->vEndScale;

	m_iTurnOption = pDesc->iTurnOption;
	m_fTurnSpeed = pDesc->fTurnSpeed;
	m_vRandomAxis = _float3(
		GetRandomFloatInRange(pDesc->vRandomAxis_Min.x, pDesc->vRandomAxis_Max.x),
		GetRandomFloatInRange(pDesc->vRandomAxis_Min.y, pDesc->vRandomAxis_Max.y),
		GetRandomFloatInRange(pDesc->vRandomAxis_Min.z, pDesc->vRandomAxis_Max.z)
	);
}

void EffectMovementScript::Tick()
{
	m_fCurrAge += fDT;

	Translate();
	Scaling();
	Turn();
}

void EffectMovementScript::InitialTransform(_float3 vParentPos, _float3 vParentScale, _float3 vParentRotation)
{
	// For. Position, Scale, Rotation 
	m_vStartPos += vParentPos;
	m_vStartScale += vParentScale;
	m_vStartRotation += vParentRotation;
	m_vEndPos += m_vStartPos;

	Get_Transform()->Set_State(Transform_State::POS, _float4(m_vStartPos, 1.f));
	Get_Transform()->Scaled(m_vStartScale);
	Get_Transform()->Set_Rotation(m_vStartRotation);
}

void EffectMovementScript::Translate()
{
	switch (m_iTranslateOption)
	{
	case 0:
		break;
	case 1: // Move to target position
	case 2: // Move to random target position
		_float3 vPos = XMVectorLerp(m_vStartPos, m_vEndPos, m_fCurrAge);
		Get_Transform()->Set_State(Transform_State::POS, _float4(vPos, 0.f));
		break;
	case 3:
		Get_Transform()->Go_Straight();
		break;
	case 4:
		Get_Transform()->Go_Backward();
		break;
	case 5:
		Get_Transform()->Go_Left();
		break;
	case 6:
		Get_Transform()->Go_Right();
		break;
	case 7:
		// TODO: Spreading dust
		break;
	case 8:
		// TODO: Scattered embers
		break;
	}
}

void EffectMovementScript::Scaling()
{
	_float3 vScale = XMVectorLerp(m_vStartScale, m_vEndScale, m_fCurrAge);
	Get_Transform()->Scaled(vScale);
}

void EffectMovementScript::Turn()
{
	Get_Transform()->Turn(m_vRandomAxis, m_fTurnSpeed);
}

_float EffectMovementScript::GetRandomFloatInRange(_float fStart, _float fEnd)
{
	uniform_real_distribution<float>	RandomInRange(fStart, fEnd);
	_float fResult = RandomInRange(m_RandomNumber);
	return fResult;
}
