#pragma once
#include "MonoBehaviour.h"
class ObjectTransformDebug :
    public MonoBehaviour
{
public:
    virtual void Tick() override;


public:
    void Set_Target(shared_ptr<GameObject> obj) { m_pTarget = obj; }
private:
    weak_ptr<GameObject> m_pTarget;
};

