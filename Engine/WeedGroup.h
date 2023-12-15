#pragma once
#include "MonoBehaviour.h"
#include "Utils.h"
class WeedGroup :
    public Component
{
public:
    WeedGroup();
    virtual ~WeedGroup();

public:
    virtual HRESULT Init() override;

public:
    virtual void Tick() override;
    virtual vector<shared_ptr<GameObject>>& Get_Weeds() { return m_Weeds; }
private:
    vector<shared_ptr<GameObject>> m_Weeds;
};




