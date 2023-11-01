#pragma once
#include "Component.h"
class CharacterController :
    public Component
{
public:
    CharacterController();
    virtual ~CharacterController();

    PxCapsuleControllerDesc& Get_ControllerDesc() { return m_ControllerDesc; }
    PxController* Get_Actor() { return m_pController; }
    void Create_Controller();

    virtual HRESULT Init() override;

    friend class Transform;

private:
    PxCapsuleControllerDesc m_ControllerDesc = PxCapsuleControllerDesc();
    PxController* m_pController = nullptr;
    PxMaterial* m_pMaterial = nullptr;

};

