#pragma once
#include "MonoBehaviour.h"

class UiDamageMove : public MonoBehaviour
{
public:
    UiDamageMove(weak_ptr<GameObject> pObj, _uint iIndex, shared_ptr<GameObject> pFirstNum = nullptr);


public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiDamageMove>(m_pTarget.lock(), 0); }


private:
    void Check_Render_State();
    void Change_Pos();
    void Check_Remove();

private:
    weak_ptr<GameObject>    m_pCamera;
    weak_ptr<GameObject>    m_pTarget;
    weak_ptr<GameObject>    m_pFirstNum;

    _float  m_fCheckTime    = {};
    _float  m_fMaxTime      = {};
    _float  m_fRatio        = {};
    _bool   m_bIsRender     = {};

    _uint   m_iIndex        = {};
    _uint   m_iNum          = {};
};

