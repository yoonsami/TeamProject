#include "pch.h"
#include "CharacterController.h"

CharacterController::CharacterController() : Component(COMPONENT_TYPE::CharacterController)
{
	
	
}

CharacterController::~CharacterController()
{
}

void CharacterController::Create_Controller(_bool IsBoxCollider)
{
	m_pMaterial = PHYSX.Get_PxPhysics()->createMaterial(0.f, 0.f, 1.f);
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
	PHYSX.Add_GameObject(Get_Owner());
	//PHYSX.Get_PxScene()->addActor(*(m_pController->getActor()));
}

HRESULT CharacterController::Init()
{
	m_pController->setFootPosition({ Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).x, Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).y, Get_Owner()->Get_Transform()->Get_State(Transform_State::POS).z });
	
	
	return S_OK;
}

void CharacterController::Tick()
{
	/*if (m_bGravityOn)
	{
		_float3 vDir = _float3(0.f, -9.8f, 0.f);

		if (!m_pController)
			return;
		auto result = m_pController->move({ vDir.x ,vDir.y  ,vDir.z }, 0.0f, fDT, PxControllerFilters());
	}*/
	auto result = m_pController->move({ 0.f ,-30.f  ,0.f }, 0.0f, fDT, PxControllerFilters());


}

void CharacterController::Add_Velocity(const _float vel)
{
	m_fYVelocity += vel;
	m_bOnGround = false;
	m_bJump = true;
}
