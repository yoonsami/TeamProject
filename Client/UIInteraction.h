#pragma once
#include "MonoBehaviour.h"

class UIInteraction : public MonoBehaviour
{
public:
    UIInteraction();

public:
    virtual HRESULT Init() override;

	virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<UIInteraction>(); }

    void Create_Interaction(NPCTYPE eType);
    void Remove_Interaction();

private:
    weak_ptr<GameObject> m_pInteraction_Bg;
    weak_ptr<GameObject> m_pInteraction_Font;
    weak_ptr<GameObject> m_pDialog_Controller;
    weak_ptr<GameObject> m_pGachaController;

    _bool           m_bIsCreated = {};
};

