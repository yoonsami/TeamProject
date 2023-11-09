#include "pch.h"
#include "CharacterController.h"

CharacterController::CharacterController() : Component(COMPONENT_TYPE::CharacterController)
{
	m_pMaterial = PHYSX.Get_PxPhysics()->createMaterial(0.f, 0.f, 1.f);
	
}

CharacterController::~CharacterController()
{

}

void CharacterController::Create_Controller(_bool IsBoxCollider)
{
	if (IsBoxCollider)
	{
		m_BoxControllerDesc.material = m_pMaterial;


		m_pController = PHYSX.Get_ControllManager()->createController(m_BoxControllerDesc);

	}
	else
	{
		m_CapsuleControllerDesc.material = m_pMaterial;


		m_pController = PHYSX.Get_ControllManager()->createController(m_CapsuleControllerDesc);
	}
	assert(m_pController);

	m_pController->getActor()->setMass(1.f);
	//PHYSX.Get_PxScene()->addActor(*(m_pController->getActor()));
}

HRESULT CharacterController::Init()
{
	m_pController->setFootPosition({ Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).x, Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).y, Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).z });
	
	
	return S_OK;
}

void CharacterController::Tick()
{
	if (m_bGravityOn)
	{
		_float3 vDir = _float3(0.f,-9.8f,0.f);

		auto result = m_pController->move({ vDir.x ,vDir.y  ,vDir.z }, 0.0f, fDT, PxControllerFilters());
	}
}
