#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"

class YeopoHorse_FSM :
	public FSM
{
public:
	enum class STATE
	{
		n_idle,
		skill_400100_fx,
		skill_902100_fx,
		NONE
	};
public:
	YeopoHorse_FSM();
	~YeopoHorse_FSM();


public:
	virtual HRESULT Init() override;
	virtual void Tick() override;


private:
	virtual void State_Tick() override; // 상태를 항상 업데이트해줌
	virtual void State_Init() override; // 상태가 바뀔 때 한번 초기화 해줌
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void Get_Hit(const wstring& skillname, shared_ptr<BaseCollider> pOppositeCollider) override;
	virtual void AttackCollider_On(const wstring& skillname) override;
	virtual void AttackCollider_Off() override;

	void n_idle();
	void n_idle_Init();
	void skill_400100_fx();
	void skill_400100_fx_Init();
	void skill_902100_fx();
	void skill_902100_fx_Init();
	
public:
	void Set_State(_uint iIndex) { m_eCurState = (STATE)iIndex; }

private:
	STATE m_eCurState = STATE::n_idle;
	STATE m_ePreState = STATE::NONE;
};

