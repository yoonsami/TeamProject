#include "pch.h"
#include "CoolTimeCheckScript.h"

#include "Material.h"
#include "MeshRenderer.h"
#include "UiCoolEndEffect.h"
#include "UiSkillButtonEffect.h"

CoolTimeCheckScript::CoolTimeCheckScript()
{
}

HRESULT CoolTimeCheckScript::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_eCurHero = HERO::PLAYER;

    m_CoolTime.resize(IDX(HERO::MAX));
    //m_TextureKey.resize(IDX(HERO::MAX));
    for (_uint i = 0; i < IDX(HERO::MAX); ++i)
    {
        m_CoolTime[i].resize(5);
        //m_TextureKey[i].resize(7);
    }

    m_tagEvade.fCoolTime = 3.f;

    m_pUiSkill.resize(7);

    m_pUiSkill[0] = CUR_SCENE->Get_UI(L"UI_Skill0");
    m_pUiSkill[1] = CUR_SCENE->Get_UI(L"UI_Skill1");
    m_pUiSkill[2] = CUR_SCENE->Get_UI(L"UI_Skill2");
    m_pUiSkill[3] = CUR_SCENE->Get_UI(L"UI_Skill3");
    m_pUiSkill[4] = CUR_SCENE->Get_UI(L"UI_Skill4");
    m_pUiSkill[5] = CUR_SCENE->Get_UI(L"UI_Skill5");
    m_pUiSkill[6] = CUR_SCENE->Get_UI(L"UI_Skill6");

    m_pUiSkill_Effect.resize(7);
    m_pUiSkill_Effect [0] = CUR_SCENE->Get_UI(L"UI_Skill2_Effect");
    m_pUiSkill_Effect [1] = CUR_SCENE->Get_UI(L"UI_Skill3_Effect");
    m_pUiSkill_Effect [2] = CUR_SCENE->Get_UI(L"UI_Skill4_Effect");
    m_pUiSkill_Effect [3] = CUR_SCENE->Get_UI(L"UI_Skill5_Effect");
    m_pUiSkill_Effect [4] = CUR_SCENE->Get_UI(L"UI_Skill6_Effect");
    m_pUiSkill_Effect [5] = CUR_SCENE->Get_UI(L"UI_Skill1_Effect");
    m_pUiSkill_Effect [6] = CUR_SCENE->Get_UI(L"UI_Skill0_Effect");

    m_pUi_Cool_End.resize(5);
    m_pUi_Cool_End[0] = CUR_SCENE->Get_UI(L"UI_Cool_End2");
    m_pUi_Cool_End[1] = CUR_SCENE->Get_UI(L"UI_Cool_End3");
    m_pUi_Cool_End[2] = CUR_SCENE->Get_UI(L"UI_Cool_End4");
    m_pUi_Cool_End[3] = CUR_SCENE->Get_UI(L"UI_Cool_End5");
    m_pUi_Cool_End[4] = CUR_SCENE->Get_UI(L"UI_Cool_End6");
        
    // Set Max Cool Time
    // 5 skill
    // 7 texture : attack, evade, skill 0 ~ 4
    // 
    


    // Player
    auto tagData = GET_DATA(HERO::PLAYER);
    m_CoolTime[IDX(HERO::PLAYER)][0].fCoolTime = tagData.Skill1Cool;
    m_CoolTime[IDX(HERO::PLAYER)][1].fCoolTime = tagData.Skill2Cool;
    m_CoolTime[IDX(HERO::PLAYER)][2].fCoolTime = tagData.Skill3Cool;
    m_CoolTime[IDX(HERO::PLAYER)][3].fCoolTime = tagData.Skill4Cool;
    m_CoolTime[IDX(HERO::PLAYER)][4].fCoolTime = tagData.Skill5Cool;
    
    // Ace
    tagData = GET_DATA(HERO::ACE3);
    m_CoolTime[IDX(HERO::ACE3)][0].fCoolTime = tagData.Skill1Cool;
    m_CoolTime[IDX(HERO::ACE3)][1].fCoolTime = tagData.Skill2Cool;
    m_CoolTime[IDX(HERO::ACE3)][2].fCoolTime = tagData.Skill3Cool;
    m_CoolTime[IDX(HERO::ACE3)][3].fCoolTime = tagData.Skill4Cool;
    m_CoolTime[IDX(HERO::ACE3)][4].fCoolTime = tagData.Skill5Cool;

    // Kyle
    tagData = GET_DATA(HERO::KYLE);
    m_CoolTime[IDX(HERO::KYLE)][0].fCoolTime = tagData.Skill1Cool;
    m_CoolTime[IDX(HERO::KYLE)][1].fCoolTime = tagData.Skill2Cool;
    m_CoolTime[IDX(HERO::KYLE)][2].fCoolTime = tagData.Skill3Cool;
    m_CoolTime[IDX(HERO::KYLE)][3].fCoolTime = tagData.Skill4Cool;
    m_CoolTime[IDX(HERO::KYLE)][4].fCoolTime = tagData.Skill5Cool;

    // Yeopo
    tagData = GET_DATA(HERO::YEOPO);
    m_CoolTime[IDX(HERO::YEOPO)][0].fCoolTime = tagData.Skill1Cool;
    m_CoolTime[IDX(HERO::YEOPO)][1].fCoolTime = tagData.Skill2Cool;
    m_CoolTime[IDX(HERO::YEOPO)][2].fCoolTime = tagData.Skill3Cool;
    m_CoolTime[IDX(HERO::YEOPO)][3].fCoolTime = tagData.Skill4Cool;
    m_CoolTime[IDX(HERO::YEOPO)][4].fCoolTime = tagData.Skill5Cool;

    // Dellons
    tagData = GET_DATA(HERO::DELLONS);
    m_CoolTime[IDX(HERO::DELLONS)][0].fCoolTime = tagData.Skill1Cool;
    m_CoolTime[IDX(HERO::DELLONS)][1].fCoolTime = tagData.Skill2Cool;
    m_CoolTime[IDX(HERO::DELLONS)][2].fCoolTime = tagData.Skill3Cool;
    m_CoolTime[IDX(HERO::DELLONS)][3].fCoolTime = tagData.Skill4Cool;
    m_CoolTime[IDX(HERO::DELLONS)][4].fCoolTime = tagData.Skill5Cool;



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

    _uint iSize = IDX(m_pUiSkill.size());
    for (_uint i = 0; i < iSize; ++i)
        if (true == m_pUiSkill[i].expired())
            return;

    /*for(_uint i = 0 ; i < iSize; ++i)
        m_pUiSkill[i].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_TextureKey[iIndex][i]), TextureMapType::DIFFUSE);*/

    auto tagData = GET_DATA(eType);
    m_pUiSkill[0].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(tagData.KeyAttack), TextureMapType::DIFFUSE);
    m_pUiSkill[1].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(tagData.KeyEvade), TextureMapType::DIFFUSE);
    m_pUiSkill[2].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(tagData.KeySkill1), TextureMapType::DIFFUSE);
    m_pUiSkill[3].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(tagData.KeySkill2), TextureMapType::DIFFUSE);
    m_pUiSkill[4].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(tagData.KeySkill3), TextureMapType::DIFFUSE);
    m_pUiSkill[5].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(tagData.KeySkill4), TextureMapType::DIFFUSE);
    m_pUiSkill[6].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(tagData.KeySkill5), TextureMapType::DIFFUSE);
}

void CoolTimeCheckScript::Set_Skill_End()
{
    //if (m_pOwner.expired() || HERO::MAX == m_eCurHero)
    //    return;

    //m_bIsSkillWork = false;2
    //m_CoolTime[IDX(m_eCurHero)][m_iWorkSkillIndex].bIsWork = false;
    //m_CoolTime[IDX(m_eCurHero)][m_iWorkSkillIndex].fAccTime = 0.f;


    if (m_pOwner.expired() || HERO::MAX == m_eCurHero)
        return;

    if (SkillType::DEFAULT == m_iWorkSkillIndex)
        return;

    m_bIsSkillWork = false;
    m_CoolTime[IDX(m_eCurHero)][m_iWorkSkillIndex].bIsWork = false;
    m_CoolTime[IDX(m_eCurHero)][m_iWorkSkillIndex].fAccTime = 0.f;

    m_iWorkSkillIndex = SkillType::DEFAULT;
}

_bool CoolTimeCheckScript::IsAvailable(SkillType eSkillType)
{
    if (m_pOwner.expired() || HERO::MAX == m_eCurHero || 5 < eSkillType)
        return false;

    // 5 : evade
    else if (5 == eSkillType)
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
    auto& CoolInfo = m_CoolTime[IDX(m_eCurHero)][eSkillType];
    if (CoolInfo.fCoolTime < CoolInfo.fAccTime)
    {
        CoolInfo.bIsEnd = false;
        CoolInfo.bIsWork = true;
        //CoolInfo.fAccTime = 0.f;
        m_bIsSkillWork = true;
        m_iWorkSkillIndex = eSkillType;

        Start_Effect(eSkillType);

        return true;
    }

    return false;
}

void CoolTimeCheckScript::Start_Attack_Button_Effect()
{
    Start_Effect(DEFAULT);
}

void CoolTimeCheckScript::Check_Cool_Time()
{
    _float fDt = fDT;
    for (_uint i = 0; i < IDX(HERO::MAX); ++i)
    {
        auto& vecCool = m_CoolTime[i];
        for (_uint j = 0; j < 5; ++j)
        {
            if (false == vecCool[j].bIsWork && vecCool[j].fCoolTime > vecCool[j].fAccTime)
            {
                vecCool[j].fAccTime += fDt;
                if (false == vecCool[j].bIsEnd && vecCool[j].fCoolTime < vecCool[j].fAccTime)
                {
                    vecCool[j].bIsEnd = true;
                    if(i == IDX(m_eCurHero))
                        Start_ButtonEndEffect(j);
                }
            }
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

    if (nullptr == pGameobjectGauge)
        return;

    for (_uint i = 0; i < IDX(m_pUiSkill.size()); ++i)
        if (true == m_pUiSkill[i].expired())
            return;

    pGameobjectGauge->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_tagEvade.fAccTime / m_tagEvade.fCoolTime;
    m_pUiSkill[2].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[0].fAccTime / vecCool[0].fCoolTime;
    m_pUiSkill[3].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[1].fAccTime / vecCool[1].fCoolTime;
    m_pUiSkill[4].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[2].fAccTime / vecCool[2].fCoolTime;
    m_pUiSkill[5].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[3].fAccTime / vecCool[3].fCoolTime;
    m_pUiSkill[6].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[4].fAccTime / vecCool[4].fCoolTime;
}

void CoolTimeCheckScript::Start_Effect(SkillType eSkillType)
{
    if (true == m_pUiSkill_Effect[eSkillType].expired())
        return;

    m_pUiSkill_Effect[eSkillType].lock()->Get_Script<UiSkillButtonEffect>()->Start_Effect();
}

void CoolTimeCheckScript::Start_ButtonEndEffect(_uint iIndex)
{
    if (true == m_pUi_Cool_End[iIndex].expired())
        return;

    m_pUi_Cool_End[iIndex].lock()->Get_Script<UiCoolEndEffect>()->Start_Effect();
}
