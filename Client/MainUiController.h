#pragma once
#include "MonoBehaviour.h"

class MainUiController : public MonoBehaviour
{
public:
    MainUiController();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Set_MainUI_Render(_bool bValue);

private:
    vector<weak_ptr<GameObject>> m_vecUi;

};

