#pragma once
#include "MonoBehaviour.h"
class DeleteWhenAnimFinish :
    public MonoBehaviour
{
public:
    DeleteWhenAnimFinish(const wstring& strAnimName, _float fWaitTime, _bool bAnimLoop = false, _float fAnimSpeed = 1.f);

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<DeleteWhenAnimFinish>(m_strAnimName,m_fWaitTime,m_bAnimLoop,m_fAnimSpeed); }


private:
    wstring m_strAnimName;
    _bool m_bAnimLoop = false;
    _float m_fAnimSpeed = 1.f;
    _float m_fWaitTime = 0.f;
    _bool m_bDissolveWhenFinished = false;
    shared_ptr<Texture> m_pTexture;

    _bool m_bAnimFinished = false;

};

