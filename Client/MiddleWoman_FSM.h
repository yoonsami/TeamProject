#pragma once
#include "Client_FSM.h"
class MiddleWoman_FSM :
    public Client_FSM
{

    enum class STATE
    {
        n_idle,
        talk,

    };
public:
	MiddleWoman_FSM(){}

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;



private:
	virtual void State_Tick() override;
	virtual void State_Init() override;

	void	n_idle();
	void	n_idle_Init();
	void	talk();
	void	talk_Init();

	virtual void Set_State(_uint iIndex) override {}
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget, _uint iElementType) override {}
	_bool Can_Interact();

private:
	STATE m_eCurState = STATE::n_idle;
	STATE m_ePreState = STATE::talk;

	_float m_fStateTimer = 0.f;
	_float3 m_vGoalPos=_float3(0.f);
};

