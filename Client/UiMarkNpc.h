#pragma once
#include "MonoBehaviour.h"

class UiMarkNpc : public MonoBehaviour
{
public:
    UiMarkNpc(NPCTYPE eType);


public:
    virtual HRESULT Init() override;
    virtual void Tick() override;


private:
    void Check_In_Screen();


private:
    weak_ptr<GameObject>    m_pCamera;
    weak_ptr<GameObject>    m_pMark;
    
    NPCTYPE     m_eType     = { NPCTYPE::MAX };


};

