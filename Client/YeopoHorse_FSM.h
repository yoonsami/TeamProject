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
		
		//Ride Horse
		SQ_RideHorse_Idle,
		SQ_RideHorse_Run,
		SQ_RideHorse_Stop,
		SQ_RideHorse_End,

		NONE
	};
public:
	YeopoHorse_FSM();
	~YeopoHorse_FSM();


public:
	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget) override;


private:
	virtual void State_Tick() override; // ���¸� �׻� ������Ʈ����
	virtual void State_Init() override; // ���°� �ٲ� �� �ѹ� �ʱ�ȭ ����
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) override;
	virtual void AttackCollider_Off() override;
	virtual void Set_State(_uint iIndex) override;

	void n_idle();
	void n_idle_Init();
	void skill_400100_fx();
	void skill_400100_fx_Init();
	void skill_902100_fx();
	void skill_902100_fx_Init();

	void SQ_RideHorse_Idle();
	void SQ_RideHorse_Idle_Init();
	void SQ_RideHorse_Run();
	void SQ_RideHorse_Run_Init();
	void SQ_RideHorse_Stop();
	void SQ_RideHorse_Stop_Init();
	void SQ_RideHorse_End();
	void SQ_RideHorse_End_Init();
	

private:
	STATE m_eCurState = STATE::n_idle;
	STATE m_ePreState = STATE::NONE;
};

