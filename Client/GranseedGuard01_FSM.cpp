#include "pch.h"
#include "GranseedGuard01_FSM.h"

GranseedGuard01_FSM::GranseedGuard01_FSM()
{
}

GranseedGuard01_FSM::~GranseedGuard01_FSM()
{
}

HRESULT GranseedGuard01_FSM::Init()
{
    return E_NOTIMPL;
}

void GranseedGuard01_FSM::Tick()
{
}

void GranseedGuard01_FSM::Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget)
{
}

void GranseedGuard01_FSM::State_Tick()
{
}

void GranseedGuard01_FSM::State_Init()
{
}

void GranseedGuard01_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void GranseedGuard01_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void GranseedGuard01_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void GranseedGuard01_FSM::AttackCollider_On(const wstring& skillname)
{
}

void GranseedGuard01_FSM::AttackCollider_Off()
{
}

void GranseedGuard01_FSM::Set_State(_uint iIndex)
{
}
