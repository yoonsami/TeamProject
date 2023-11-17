#pragma once
#include "MonoBehaviour.h"

class UiDamageMove : public MonoBehaviour
{
public:
    UiDamageMove();


public:
    virtual HRESULT Init() override;
    virtual void Tick() override;


private:
    //void Check_Render_State();
    void Change_Size();
    void Change_Alpha();
    void Check_Remove();

private:
    //weak_ptr<GameObject>    m_pCamera;

    _float  m_fOriginSize   = {};
    _float  m_fMinSize      = {};
    _float  m_fChangeScale  = {};

    _float  m_fCheckTime    = {};
    _float  m_fMaxTime      = {};
    _float  m_fRatio        = {};
    _bool   m_bIsRender     = {};
};

