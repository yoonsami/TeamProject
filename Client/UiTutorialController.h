#pragma once
#include "MonoBehaviour.h"

class UiTutorialController : public MonoBehaviour
{
public:
    UiTutorialController(_bool bValue = false);


public:
    virtual HRESULT Init() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiTutorialController>(); }

    virtual void Tick() override;

    void Start_Tutorial();

private:
    void Change_Texture();
    void Finish_Tutorial();
    void End_Tutorial();

private:
    vector<weak_ptr<Texture>>       m_pTexture;
    weak_ptr<GameObject>            m_pImage;

    _bool                           m_bIsStart  = {};
    _uint                           m_iIndex    = {};

    _bool                           m_bIsEnd    = {};

    _bool                           m_bIsOnlyCounterTutorial = {};
};

