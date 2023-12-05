#pragma once
#include "FSM.h"

struct Gacha_FSM_Desc
{
	wstring strWeaponName = L"";
	wstring strAnimTag =L"";
	_uint iAnimStopFrame = 0;
};

class Gacha_FSM :
    public FSM
{
	
	enum class STATE
	{
		SQ_SpecialHero,
		NONE,
	};
public:
	Gacha_FSM(const Gacha_FSM_Desc& desc);

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;


private:
	virtual void State_Tick() override; // ���¸� �׻� ������Ʈ����
	virtual void State_Init() override; // ���°� �ٲ� �� �ѹ� �ʱ�ȭ ����

	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) {};
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) {};
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) {};
	virtual void Set_State(_uint iIndex) {};
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget) {};

	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) {};
	virtual void AttackCollider_Off() {};
private:
	void SQ_SpecialHero();
	void SQ_SpecialHero_Init();

private:
	STATE m_eCurState = STATE::SQ_SpecialHero;
	STATE m_ePreState = STATE::NONE;

private:
	Gacha_FSM_Desc m_Desc{};
	_float m_fAnimSpeed = 1.f;
	_float m_fAcc = 0.f;
};

