#pragma once
#include "MonoBehaviour.h"

class UiTutorialController : public MonoBehaviour
{
public:
    UiTutorialController();


public:
    virtual HRESULT Init() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiTutorialController>(); }

    virtual void Tick() override;


private:


private:
    vector<weak_ptr<Texture>>       m_pTexture;


};

