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

    void Push_Weed(shared_ptr<GameObject> _pWeed) { m_Weeds.push_back(_pWeed); }

public:
    virtual void Tick() override;
    virtual vector<shared_ptr<GameObject>>& Get_Weeds() { return m_Weeds; }
private:
    vector<shared_ptr<GameObject>> m_Weeds;
};




