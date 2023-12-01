#pragma once
#include "MonoBehaviour.h"

class UiMarkNpc : public MonoBehaviour
{
public:
    UiMarkNpc(NPCTYPE eType);


public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<UiMarkNpc>(m_eType); }
    void Delete_Mark();
    void Change_Render(_bool bValue);

private:
    void Check_In_Screen();
    void Update_Pos();

private:
    weak_ptr<GameObject>    m_pCamera;
    weak_ptr<GameObject>    m_pMark;
    
    NPCTYPE     m_eType     = { NPCTYPE::MAX };


};

