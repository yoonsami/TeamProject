#include "pch.h"
#include "Component.h"
#include "GameObject.h"


Component::Component(COMPONENT_TYPE eType)
	: m_eType(eType)
{
}

Component::~Component()
{
}

shared_ptr<Transform> Component::Get_Transform()
{
	return m_pOwner.lock()->Get_Transform();
}

shared_ptr<CharacterController> Component::Get_CharacterController()
{
	return m_pOwner.lock()->Get_CharacterController();
}
