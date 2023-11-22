#pragma once
#include "MonoBehaviour.h"
class ObjectDissolve :
    public MonoBehaviour
{
public:
    ObjectDissolve(_float fDissolveSpeed, shared_ptr<Texture> dissolveTexture = nullptr);

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;


public:
    weak_ptr<Texture> m_pTexture;
    _float m_fDissolveSpeed = 0.f;

};

