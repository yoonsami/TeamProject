#pragma once
#include "MonoBehaviour.h"
#include "MeshEffectData.h"

class EffectMovementScript final : public MonoBehaviour
{
public:
	virtual void	Init(void* pArg);
	virtual void	Tick() override;

	void			InitialTransform(_float3 vParentPos, _float3 vParentScale, _float3 vParentRotation);

private:
	void			Translate();
	void			Scaling();
	void			Turn();

	_float			GetRandomFloatInRange(_float fStart, _float fEnd);

private:
	_float		m_fCurrAge = { 0.f };

	/* Initalize Transform */
	_float3		m_vStartPos;
	_float3		m_vStartScale;
	_float3		m_vStartRotation;

	/* Translate */
	_int        m_iTranslateOption;
	_float      m_fTranslateSpeed;
	_float3     m_vEndPos;


	_float3     m_vEndScale;

	_int        m_iTurnOption;
	_float      m_fTurnSpeed;
	_float3     m_vRandomAxis;

	mt19937_64						m_RandomNumber;
};

