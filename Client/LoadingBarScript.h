#pragma once
#include "MonoBehaviour.h"

class LoadingBarScript : public MonoBehaviour
{
public:
    LoadingBarScript();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<LoadingBarScript>(); }

    void Set_Cur_Load_Index(_uint iIndex) { m_iCurLoadIndex = iIndex; }

private:
    void Change_Text();

private:
    _bool   m_bIsLoadEnd        = {};

    vector<weak_ptr<GameObject>> m_addedObj;

    _uint   m_iCurLoadIndex     = {};

    _uint   m_iLoadingBarIndex  = {};
    _uint   m_iTextIndex        = {};
    _uint   m_iNumIndex         = {};

    _uint   m_iTargetNum        = {};
    _uint   m_iCurNum           = {};

    _float  m_fCheckTime        = {};
    _float  m_fMaxTime          = {};
    _uint   m_iTextDotCount     = {};
};

