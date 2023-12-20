#pragma once
#include "MonoBehaviour.h"
class Add_Score :
    public MonoBehaviour
{
public:
    Add_Score(SCORE_TYPE eTYPE):m_eType(eTYPE){}
    ~Add_Score() { SWITCHMGR.Add_Score(m_eType); }


public:
    virtual shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<Add_Score>(m_eType); }

private:
    SCORE_TYPE m_eType = SCORE_TYPE::MAX;

};

