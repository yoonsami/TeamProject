#pragma once
#include "MonoBehaviour.h"

class UIInteraction : public MonoBehaviour
{
public:
    UIInteraction();

public:
    virtual HRESULT Init() override;

	virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<UIInteraction>(); }

    void Create_Interaction(NPCTYPE eType, shared_ptr<GameObject> pObj);
    void Remove_Interaction(shared_ptr<GameObject> pObj);

    _bool Get_Is_Activate();
    _bool Is_Created()      { return m_bIsCreated; }

private:
    void Remove_Interaction();

private:
    weak_ptr<GameObject> m_pInteraction_Bg;
    weak_ptr<GameObject> m_pInteraction_Font;
    weak_ptr<GameObject> m_pDialog_Controller;
    weak_ptr<GameObject> m_pGachaController;

    weak_ptr<GameObject> m_pAccessObj;

    _bool           m_bIsCreated    = {};
    _bool           m_bIsActivate   = {};
};

