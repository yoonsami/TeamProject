#pragma once
#include "Client_FSM.h"
class FisherMan_FSM :
    public Client_FSM
{

    enum class STATE
    {
        n_idle,
        n_walk,
        talk_01,

    };
public:
	FisherMan_FSM(const _float3& vGoalPos) : m_vGoalPos(vGoalPos) {}

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;



private:
	virtual void State_Tick() override;
	virtual void State_Init() override;

	void	n_idle();
	void	n_idle_Init();
	void	n_walk();
	void	n_walk_Init();
	void	talk_01();
	void	talk_01_Init();

	virtual void Set_State(_uint iIndex) override {}
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget, _uint iElementType) override {}
	_bool Can_Interact();

private:
	STATE m_eCurState = STATE::n_walk;
	STATE m_ePreState = STATE::n_idle;

	_float m_fStateTimer = 0.f;
	_float3 m_vGoalPos=_float3(0.f);
};

