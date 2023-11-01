#include "pch.h"
#include "CharacterController.h"

CharacterController::CharacterController() : Component(COMPONENT_TYPE::CharacterController)
{
	m_pMaterial = PHYSX.Get_PxPhysics()->createMaterial(0.f, 0.f, 1.f);
	
}

CharacterController::~CharacterController()
{

}

void CharacterController::Create_Controller()
{
	m_ControllerDesc.material = m_pMaterial;


	m_pController = PHYSX.Get_ControllManager()->createController(m_ControllerDesc);
	assert(m_pController);

	//PHYSX.Get_PxScene()->addActor(*(m_pController->getActor()));
}

HRESULT CharacterController::Init()
{
	m_pController->setFootPosition({ Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).x, Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).y, Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).z });
	
	
	return S_OK;
}