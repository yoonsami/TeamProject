#pragma once
#include "MonoBehaviour.h"

class UiDialogController : public MonoBehaviour
{
public:
    UiDialogController();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Create_Dialog(NPCTYPE eType);
    void Next_Dialog();

private:
    void Remove_Dialog();
    void Move_Next();
    void Next_Down();
    void Next_Up();

private:
    weak_ptr<GameObject> m_pNext;
    weak_ptr<GameObject> m_pNpcName;
    weak_ptr<GameObject> m_pNpcBg;
    weak_ptr<GameObject> m_pNpcDialog;
    weak_ptr<GameObject> m_pPlayerBg;
    weak_ptr<GameObject> m_pPlayerDialog;

    enum class NEXT_STATE { DOWN, UP };
    NEXT_STATE      m_eState        = { NEXT_STATE::DOWN };
    _float          m_fCheckTime    = {};
    _float          m_fMaxTime      = {};
    _float          m_fSpeed        = {};

    _uint           m_iQuestCount   = {};
    _uint           m_iCurIndex     = {};
    _uint           m_iMaxIndex     = {};
    _bool           m_bIsCreated    = {};
};

