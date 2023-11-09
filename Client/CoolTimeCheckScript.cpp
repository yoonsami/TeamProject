#include "pch.h"
#include "CoolTimeCheckScript.h"

#include "Material.h"
#include "MeshRenderer.h"
#include "UiSkillButtonEffect.h"

CoolTimeCheckScript::CoolTimeCheckScript()
{
}

HRESULT CoolTimeCheckScript::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_CoolTime.resize(IDX(HERO::MAX));
    m_TextureKey.resize(IDX(HERO::MAX));
    for (_uint i = 0; i < IDX(HERO::MAX); ++i)
    {
        m_CoolTime[i].resize(5);
        m_TextureKey[i].resize(7);
    }

    m_tagEvade.fCoolTime = 3.f;

    // Set Max Cool Time
    // 5 skill
    // 7 texture : attack, evade, skill 0 ~ 4
    // Ace
    m_CoolTime[IDX(HERO::ACE)][0].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::ACE)][1].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::ACE)][2].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::ACE)][3].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::ACE)][4].fCoolTime = 5.f;

    m_TextureKey[IDX(HERO::ACE)][0] = L"Skill_Ace0";
    m_TextureKey[IDX(HERO::ACE)][1] = L"Skill_Ace1";
    m_TextureKey[IDX(HERO::ACE)][2] = L"Skill_Ace2";
    m_TextureKey[IDX(HERO::ACE)][3] = L"Skill_Ace3";
    m_TextureKey[IDX(HERO::ACE)][4] = L"Skill_Ace4";
    m_TextureKey[IDX(HERO::ACE)][5] = L"Skill_Ace5";
    m_TextureKey[IDX(HERO::ACE)][6] = L"Skill_Ace6";
    

    // Kyle
    m_CoolTime[IDX(HERO::KYLE)][0].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::KYLE)][1].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::KYLE)][2].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::KYLE)][3].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::KYLE)][4].fCoolTime = 5.f;

    m_TextureKey[IDX(HERO::KYLE)][0] = L"Skill_Kyle0";
    m_TextureKey[IDX(HERO::KYLE)][1] = L"Skill_Kyle1";
    m_TextureKey[IDX(HERO::KYLE)][2] = L"Skill_Kyle2";
    m_TextureKey[IDX(HERO::KYLE)][3] = L"Skill_Kyle3";
    m_TextureKey[IDX(HERO::KYLE)][4] = L"Skill_Kyle4";
    m_TextureKey[IDX(HERO::KYLE)][5] = L"Skill_Kyle5";
    m_TextureKey[IDX(HERO::KYLE)][6] = L"Skill_Kyle6";


    // Yeopo
    m_CoolTime[IDX(HERO::YEOPO)][0].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::YEOPO)][1].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::YEOPO)][2].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::YEOPO)][3].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::YEOPO)][4].fCoolTime = 5.f;

    m_TextureKey[IDX(HERO::YEOPO)][0] = L"Skill_Yeopo0";
    m_TextureKey[IDX(HERO::YEOPO)][1] = L"Skill_Yeopo1";
    m_TextureKey[IDX(HERO::YEOPO)][2] = L"Skill_Yeopo2";
    m_TextureKey[IDX(HERO::YEOPO)][3] = L"Skill_Yeopo3";
    m_TextureKey[IDX(HERO::YEOPO)][4] = L"Skill_Yeopo4";
    m_TextureKey[IDX(HERO::YEOPO)][5] = L"Skill_Yeopo5";
    m_TextureKey[IDX(HERO::YEOPO)][6] = L"Skill_Yeopo6";


    // Dellons
    m_CoolTime[IDX(HERO::DELLONS)][0].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::DELLONS)][1].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::DELLONS)][2].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::DELLONS)][3].fCoolTime = 5.f;
    m_CoolTime[IDX(HERO::DELLONS)][4].fCoolTime = 5.f;

    m_TextureKey[IDX(HERO::DELLONS)][0] = L"Skill_Dellons0";
    m_TextureKey[IDX(HERO::DELLONS)][1] = L"Skill_Dellons1";
    m_TextureKey[IDX(HERO::DELLONS)][2] = L"Skill_Dellons2";
    m_TextureKey[IDX(HERO::DELLONS)][3] = L"Skill_Dellons3";
    m_TextureKey[IDX(HERO::DELLONS)][4] = L"Skill_Dellons4";
    m_TextureKey[IDX(HERO::DELLONS)][5] = L"Skill_Dellons5";
    m_TextureKey[IDX(HERO::DELLONS)][6] = L"Skill_Dellons6";





    return S_OK;
}

void CoolTimeCheckScript::Tick()
{
	if (m_pOwner.expired() || HERO::MAX == m_eCurHero)
		return;

    Check_Cool_Time();

    Change_Skills_Value();

}

void CoolTimeCheckScript::Set_Cur_Hero(HERO eType)
{
    if (m_pOwner.expired())
        return;

    m_eCurHero = eType;
    _uint iIndex = IDX(m_eCurHero);

    auto pGameobject0 = CUR_SCENE->Get_GameObject(L"UI_Skill0");
    auto pGameobject1 = CUR_SCENE->Get_GameObject(L"UI_Skill1");
    auto pGameobject2 = CUR_SCENE->Get_GameObject(L"UI_Skill2");
    auto pGameobject3 = CUR_SCENE->Get_GameObject(L"UI_Skill3");
    auto pGameobject4 = CUR_SCENE->Get_GameObject(L"UI_Skill4");
    auto pGameobject5 = CUR_SCENE->Get_GameObject(L"UI_Skill5");
    auto pGameobject6 = CUR_SCENE->Get_GameObject(L"UI_Skill6");

    if (nullptr == pGameobject0 || nullptr == pGameobject1 || nullptr == pGameobject2 || nullptr == pGameobject3 || nullptr == pGameobject4 || nullptr == pGameobject5 || nullptr == pGameobject6)
        return;

    pGameobject0->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][0]), TextureMapType::DIFFUSE);
    pGameobject1->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][1]), TextureMapType::DIFFUSE);
    pGameobject2->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][2]), TextureMapType::DIFFUSE);
    pGameobject3->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][3]), TextureMapType::DIFFUSE);
    pGameobject4->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][4]), TextureMapType::DIFFUSE);
    pGameobject5->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][5]), TextureMapType::DIFFUSE);
    pGameobject6->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][6]), TextureMapType::DIFFUSE);
}

void CoolTimeCheckScript::Set_Skill_End()
{
    if (m_pOwner.expired() || HERO::MAX == m_eCurHero)
        return;

    m_bIsSkillWork = false;
    m_CoolTime[IDX(m_eCurHero)][m_iWorkSkillIndex].bIsWork = false;
}

_bool CoolTimeCheckScript::IsAvailable(_uint iSkillIndex)
{
    if (m_pOwner.expired() || HERO::MAX == m_eCurHero || 5 < iSkillIndex)
        return false;

    // 5 : evade
    else if (5 == iSkillIndex)
    {
        if (m_tagEvade.fCoolTime < m_tagEvade.fAccTime)
        {
            m_tagEvade.fAccTime = 0.f;

            return true;
        }
        else
        {
            return false;
        }
    }

    // 0 ~ 4 : skill 
    auto& CoolInfo = m_CoolTime[IDX(m_eCurHero)][iSkillIndex];
    if (CoolInfo.fCoolTime < CoolInfo.fAccTime)
    {
        CoolInfo.bIsWork = true;
        CoolInfo.fAccTime = 0.f;
        m_bIsSkillWork = true;
        m_iWorkSkillIndex = iSkillIndex;

        Start_Effect(iSkillIndex);

        return true;
    }

    return false;
}

void CoolTimeCheckScript::Start_Attack_Button_Effect()
{
    Start_Effect(5);
}

void CoolTimeCheckScript::Check_Cool_Time()
{
    _float fDt = fDT;
    for (_uint i = 0; i < IDX(HERO::MAX); ++i)
    {
        auto& vecCool = m_CoolTime[i];
        for (_uint j = 0; j < 5; ++j)
        {
            if (/*false == vecCool[j].bIsWork &&*/ vecCool[j].fCoolTime > vecCool[j].fAccTime)
                vecCool[j].fAccTime += fDt;
        }
    }

    if (m_tagEvade.fCoolTime > m_tagEvade.fAccTime)
        m_tagEvade.fAccTime += fDt;
}

void CoolTimeCheckScript::Change_Skills_Value()
{
    if (m_pOwner.expired() || HERO::MAX == m_eCurHero)
        return;

    auto& vecCool = m_CoolTime[IDX(m_eCurHero)];

    auto pGameobjectGauge = CUR_SCENE->Get_GameObject(L"UI_Skill_Gauge");
    auto pGameobject2 = CUR_SCENE->Get_GameObject(L"UI_Skill2");
    auto pGameobject3 = CUR_SCENE->Get_GameObject(L"UI_Skill3");
    auto pGameobject4 = CUR_SCENE->Get_GameObject(L"UI_Skill4");
    auto pGameobject5 = CUR_SCENE->Get_GameObject(L"UI_Skill5");
    auto pGameobject6 = CUR_SCENE->Get_GameObject(L"UI_Skill6");

    if (nullptr == pGameobjectGauge || nullptr == pGameobject2 || nullptr == pGameobject3 || nullptr == pGameobject4 || nullptr == pGameobject5 || nullptr == pGameobject6)
        return;

    pGameobjectGauge->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_tagEvade.fAccTime / m_tagEvade.fCoolTime;
    pGameobject2->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[0].fAccTime / vecCool[0].fCoolTime;
    pGameobject3->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[1].fAccTime / vecCool[1].fCoolTime;
    pGameobject4->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[2].fAccTime / vecCool[2].fCoolTime;
    pGameobject5->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[3].fAccTime / vecCool[3].fCoolTime;
    pGameobject6->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[4].fAccTime / vecCool[4].fCoolTime;
}

void CoolTimeCheckScript::Start_Effect(_uint iIndex)
{
    wstring strName;
    switch (iIndex)
    {
    case 0:
        strName = L"UI_Skill2_Effect";
        break;
    case 1:
        strName = L"UI_Skill3_Effect";
        break;
    case 2:
        strName = L"UI_Skill4_Effect";
        break;
    case 3:
        strName = L"UI_Skill5_Effect";
        break;
    case 4:
        strName = L"UI_Skill6_Effect";
        break;
    case 5:
        strName = L"UI_Skill0_Effect";
        break;
    }

    CUR_SCENE->Get_GameObject(strName)->Get_Script<UiSkillButtonEffect>()->Start_Effect();
}
