#pragma once
#include "Component.h"
class CharacterController :
    public Component
{
public:
    CharacterController();
    virtual ~CharacterController();

    PxCapsuleControllerDesc& Get_CapsuleControllerDesc() { return m_CapsuleControllerDesc; }
    PxCapsuleControllerDesc& Get_BoxControllerDesc() { return m_CapsuleControllerDesc; }

    PxController* Get_Actor() { return m_pController; }
    void Create_Controller(_bool IsBoxCollider = false);

    virtual HRESULT Init() override;
    virtual void Tick() override;
    void Set_GravityOn(_bool flag) { m_bGravityOn = flag; }
    friend class Transform;

private:
    PxCapsuleControllerDesc m_CapsuleControllerDesc = PxCapsuleControllerDesc();
    PxBoxControllerDesc m_BoxControllerDesc = PxBoxControllerDesc();
    PxController* m_pController = nullptr;
    PxMaterial* m_pMaterial = nullptr;
    _bool m_bGravityOn = true;
};

