#include "pch.h"
#include "AttackColliderInfoScript.h"

void AttackColliderInfoScript::Tick()
{


}

void AttackColliderInfoScript::Set_ColliderOwner(shared_ptr<GameObject> pColliderOwner)
{
	m_pColliderOwner = pColliderOwner; 
}


