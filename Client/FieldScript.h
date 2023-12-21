#pragma once
#include "MonoBehaviour.h"
class FieldScript :
    public MonoBehaviour
{
public:
    FieldScript(shared_ptr<GameObject> pPlayer, _float fDetectRange);

public:
    virtual shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<FieldScript>(m_pPlayer.lock(),m_fDetectRange); }
    virtual void Tick() override;


private:
	void Load_Monster(_uint iCnt, const wstring& strMonsterTag, shared_ptr<GameObject> pPlayer);

private:
    weak_ptr<GameObject> m_pPlayer;
    _float m_fDetectRange = 0.f;
};

