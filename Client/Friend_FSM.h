#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"
#include "InstallationSkill_Script.h"

class Friend_FSM :
    public FSM
{
public:
	enum class STATE
	{
		fall_loop,
		fall_end,
		ATTACK,
		EXIT,
		NONE
	};

public:
    Friend_FSM(HERO eType);

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget) override;

private:
	virtual void State_Tick() override;
	virtual void State_Init() override;
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override {};
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override {};
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override {};
	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) override {};
	virtual void AttackCollider_Off() override {};
	virtual void Set_State(_uint iIndex) override;

	void Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage);
	void Create_InstallationSkillCollider(const _float4& vPos, _float fSkillRange, INSTALLATIONSKILLDESC desc);

	void Summon_Wraith();
	void Set_WraithState(_uint iAnimindex);

private:
	void	fall_loop();
	void	fall_loop_Init();
	void	fall_end();
	void	fall_end_Init();
	void	ATTACK();
	void	ATTACK_Init();
	void	EXIT();
	void	EXIT_Init();

	STATE m_eCurState = STATE::fall_loop;
	STATE m_ePreState = STATE::NONE;

private:
	_float	m_fBottomHeight = 0.f;
	HERO	m_eType = HERO::MAX;
	_float	m_fStateAcc = 0.f;
	weak_ptr<GameObject> m_pDellonsWraith;
};
