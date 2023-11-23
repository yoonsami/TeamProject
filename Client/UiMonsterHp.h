#pragma once
#include "MonoBehaviour.h"

class UiMonsterHp : public MonoBehaviour
{
public:
    UiMonsterHp();


public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    //void Set_Target(shared_ptr<GameObject> pObj);

private:
    void Check_Render_State();
    void Check_Target();
    void Change_Hp_Ratio();
    void Change_Hp_Slow();
    void Update_Target_Pos();

private:
    weak_ptr<GameObject>    m_pFrontHp;
    weak_ptr<GameObject>    m_pBackHp;
    weak_ptr<GameObject>    m_pBgHp;
    weak_ptr<GameObject>    m_pCamera;

    _float  m_fRatio            = { 0.f };
    _float  m_fTargetRatio      = { 0.f };
    _float  m_fSpeed            = { 0.f };
    _bool   m_bIsWork           = { false };
    _bool   m_bIsRender         = { false };
};


