#pragma once
#include "MonoBehaviour.h"

class UiCostumeController : public MonoBehaviour
{
public:
    enum class COSTUME_TYPE { HAIR, UNIFORM };

public:
    UiCostumeController();

public:
    virtual HRESULT Init() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiCardDeckController>(); }

    virtual void Tick() override;

    void Create_Costume();
    void Remove_Costume();
    void Change_Costume_Type(COSTUME_TYPE eType);

private:


private:
    vector<weak_ptr<GameObject>>    m_vecAddedObj;

    COSTUME_TYPE    m_eType = { COSTUME_TYPE::HAIR };
    _bool           m_bIsCreated        = {};

    vector<pair<wstring, wstring>>  m_vecHair;
    vector<pair<wstring, wstring>>  m_vecUniform;


};

