#pragma once
#include "MonoBehaviour.h"

class LoadingBarScript : public MonoBehaviour
{
public:
    LoadingBarScript();
    ~LoadingBarScript();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<LoadingBarScript>(); }

    void    Set_Cur_Load_Index(_uint iIndex) { m_iCurLoadIndex = iIndex; }
    _bool   Is_Load_End()   { return m_bIsLoadEnd; }

private:
    void Change_Text();
    void Check_Time();

private:
    static _uint    iStringIndex;
    _bool   m_bIsLoadEnd        = {};
    _bool   m_bIsEndActivated   = {};
    _float  m_fCheckEndTime     = {};

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

    _float  m_fAddValueTime     = {};
    _float  m_fMaxAddTime       = {};

    vector<pair<_float, wstring>> m_vecString;
};

