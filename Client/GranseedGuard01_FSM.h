#pragma once
#include "FSM.h"
class GranseedGuard01_FSM :
    public FSM
{
public:
	enum class STATE
	{
		b_idle_02,
		b_idle_03,
		b_idle_04,
		b_idle_05,
		b_idle_06,
		b_idle_07,
		b_idle_08,
		b_idle_09,
		n_idle,
		run,
		talk,
		walk,
		
		NONE
	};
public:
	GranseedGuard01_FSM();
	~GranseedGuard01_FSM();


public:
	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual void Get_Hit(const wstring& skillname,_float fDamage, shared_ptr<GameObject> pLookTarget) override;

private:
	virtual void State_Tick() override;
	virtual void State_Init() override;
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) override;
	virtual void AttackCollider_Off() override;
	virtual void Set_State(_uint iIndex) override;


};

