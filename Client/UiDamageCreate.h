#pragma once
#include "MonoBehaviour.h"

class UiDamageCreate : public MonoBehaviour
{
public:
    UiDamageCreate();


public:
    virtual HRESULT Init() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiDamageCreate>(); }

    void Create_Damage_Font(weak_ptr<GameObject> pTarget, _float fDamage, ElementType eType);
    void Create_Damage_Hit();

private:
    weak_ptr<GameObject>    m_pPlayer;
    weak_ptr<GameObject>    m_pCamera;

};

