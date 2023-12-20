#pragma once
#include "Client_FSM.h"
class Feni_FSM :
    public Client_FSM
{
    enum class STATE
    {
        SQ_afraid_01,
        SQ_afraid_02,
        SQ_afraid_03,
        SQ_afraid_idle,
        b_idle,
        b_run,
        n_idle,
        n_run,
        SQ_Surprise,
        talk,
        talk_03,
        talk02
    };

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;



private:
	virtual void State_Tick() override;
	virtual void State_Init() override;

	void    SQ_afraid_01();
	void    SQ_afraid_01_Init();
	void	SQ_afraid_02();
	void	SQ_afraid_02_Init();
	void	SQ_afraid_03();
	void	SQ_afraid_03_Init();
	void	SQ_afraid_idle();
	void	SQ_afraid_idle_Init();
	void	b_idle();
	void	b_idle_Init();
	void	b_run();
	void	b_run_Init();
	void	n_idle();
	void	n_idle_Init();
	void	n_run();
	void	n_run_Init();
	void	SQ_Surprise();
	void	SQ_Surprise_Init();
	void	talk();
	void	talk_Init();
	void	talk_03();
	void	talk_03_Init();
	void	talk02();
	void	talk02_Init();

	virtual void Set_State(_uint iIndex) override {}
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget, _uint iElementType) override {}

private:
	STATE m_eCurState = STATE::SQ_afraid_idle;
	STATE m_ePreState = STATE::SQ_afraid_01;

	_float m_fStateTimer = 0.f;
};

