#include "pch.h"
#include "CoolTimeCheckScript.h"

#include "Material.h"
#include "MeshRenderer.h"
#include "FontRenderer.h"
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
    for (_uint i = 0; i < IDX(HERO::MAX); ++i)
    {
        m_CoolTime[i].resize(5);

        auto tagData = GET_DATA(static_cast<HERO>(i));
       m_CoolTime[i][0].fCoolTime = tagData.Skill1Cool;
       m_CoolTime[i][1].fCoolTime = tagData.Skill2Cool;
       m_CoolTime[i][2].fCoolTime = tagData.Skill3Cool;
       m_CoolTime[i][3].fCoolTime = tagData.Skill4Cool;
       m_CoolTime[i][4].fCoolTime = tagData.Skill5Cool;
       m_CoolTime[i][0].fAccTime = m_CoolTime[i][0].fCoolTime + 0.1f;
       m_CoolTime[i][1].fAccTime = m_CoolTime[i][1].fCoolTime + 0.1f;
       m_CoolTime[i][2].fAccTime = m_CoolTime[i][2].fCoolTime + 0.1f;
       m_CoolTime[i][3].fAccTime = m_CoolTime[i][3].fCoolTime + 0.1f;
       m_CoolTime[i][4].fAccTime = m_CoolTime[i][4].fCoolTime + 0.1f;
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
    m_pUiSkill_Effect[0] = CUR_SCENE->Get_UI(L"UI_Skill2_Effect");
    m_pUiSkill_Effect[1] = CUR_SCENE->Get_UI(L"UI_Skill3_Effect");
    m_pUiSkill_Effect[2] = CUR_SCENE->Get_UI(L"UI_Skill4_Effect");
    m_pUiSkill_Effect[3] = CUR_SCENE->Get_UI(L"UI_Skill5_Effect");
    m_pUiSkill_Effect[4] = CUR_SCENE->Get_UI(L"UI_Skill6_Effect");
    m_pUiSkill_Effect[5] = CUR_SCENE->Get_UI(L"UI_Skill1_Effect");
    m_pUiSkill_Effect[6] = CUR_SCENE->Get_UI(L"UI_Skill0_Effect");

    m_pUi_Cool_End.resize(5);
    m_pUi_Cool_End[0] = CUR_SCENE->Get_UI(L"UI_Cool_End2");
    m_pUi_Cool_End[1] = CUR_SCENE->Get_UI(L"UI_Cool_End3");
    m_pUi_Cool_End[2] = CUR_SCENE->Get_UI(L"UI_Cool_End4");
    m_pUi_Cool_End[3] = CUR_SCENE->Get_UI(L"UI_Cool_End5");
    m_pUi_Cool_End[4] = CUR_SCENE->Get_UI(L"UI_Cool_End6");

    m_pUi_Skill_Cool.resize(5);
    m_pUi_Skill_Cool[0] = CUR_SCENE->Get_UI(L"UI_Skill2_Cool");
    m_pUi_Skill_Cool[1] = CUR_SCENE->Get_UI(L"UI_Skill3_Cool");
    m_pUi_Skill_Cool[2] = CUR_SCENE->Get_UI(L"UI_Skill4_Cool");
    m_pUi_Skill_Cool[3] = CUR_SCENE->Get_UI(L"UI_Skill5_Cool");
    m_pUi_Skill_Cool[4] = CUR_SCENE->Get_UI(L"UI_Skill6_Cool");
    for (_uint i = 0; i < 5; ++i)
        m_pUi_Skill_Cool[i].lock()->Get_FontRenderer()->Get_Text() = L"";

    m_bIsCoolChangeOne.resize(5);
    m_bIsCoolChangeOne[0] = false;
    m_bIsCoolChangeOne[1] = false;
    m_bIsCoolChangeOne[2] = false;
    m_bIsCoolChangeOne[3] = false;
    m_bIsCoolChangeOne[4] = false;



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

    for (_uint i = 0; i < IDX(m_pUi_Skill_Cool.size()); ++i)
        if (true == m_pUi_Skill_Cool[i].expired())
            return;


    for (_uint i = 0; i < IDX(m_pUi_Skill_Cool.size()); ++i)
    {
        Change_Pos_Two_Letter(i);
    }
    
    auto& vecCool = m_CoolTime[IDX(eType)];
    _uint iSkill1Cool = IDX(vecCool[0].fCoolTime - vecCool[0].fAccTime + 1.f);
    _uint iSkill2Cool = IDX(vecCool[1].fCoolTime - vecCool[1].fAccTime + 1.f);
    _uint iSkill3Cool = IDX(vecCool[2].fCoolTime - vecCool[2].fAccTime + 1.f);
    _uint iSkill4Cool = IDX(vecCool[3].fCoolTime - vecCool[3].fAccTime + 1.f);
    _uint iSkill5Cool = IDX(vecCool[4].fCoolTime - vecCool[4].fAccTime + 1.f);

    if (0 < iSkill1Cool && iSkill1Cool < 10 && false == m_bIsCoolChangeOne[0])
        Change_Pos_One_Letter(0);
    if (0 < iSkill2Cool && iSkill2Cool < 10 && false == m_bIsCoolChangeOne[1])
        Change_Pos_One_Letter(1);
    if (0 < iSkill3Cool && iSkill3Cool < 10 && false == m_bIsCoolChangeOne[2])
        Change_Pos_One_Letter(2);
    if (0 < iSkill4Cool && iSkill4Cool < 10 && false == m_bIsCoolChangeOne[3])
        Change_Pos_One_Letter(3);
    if (0 < iSkill5Cool && iSkill5Cool < 10 && false == m_bIsCoolChangeOne[4])
        Change_Pos_One_Letter(4);

    if (0 < iSkill1Cool)
        m_pUi_Skill_Cool[0].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill1Cool);
    if (0 < iSkill2Cool)
        m_pUi_Skill_Cool[1].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill2Cool);
    if (0 < iSkill3Cool)
        m_pUi_Skill_Cool[2].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill3Cool);
    if (0 < iSkill4Cool)
        m_pUi_Skill_Cool[3].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill4Cool);
    if (0 < iSkill5Cool)
        m_pUi_Skill_Cool[4].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill5Cool);
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
                    Change_Pos_Two_Letter(j);
                    
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

    for (_uint i = 0; i < IDX(m_pUi_Skill_Cool.size()); ++i)
        if (true == m_pUi_Skill_Cool[i].expired())
            return;

    pGameobjectGauge->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_tagEvade.fAccTime / m_tagEvade.fCoolTime;
    m_pUiSkill[2].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[0].fAccTime / vecCool[0].fCoolTime;
    m_pUiSkill[3].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[1].fAccTime / vecCool[1].fCoolTime;
    m_pUiSkill[4].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[2].fAccTime / vecCool[2].fCoolTime;
    m_pUiSkill[5].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[3].fAccTime / vecCool[3].fCoolTime;
    m_pUiSkill[6].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = vecCool[4].fAccTime / vecCool[4].fCoolTime;

    
    _uint iSkill1Cool = IDX(vecCool[0].fCoolTime - vecCool[0].fAccTime + 1.f);
    _uint iSkill2Cool = IDX(vecCool[1].fCoolTime - vecCool[1].fAccTime + 1.f);
    _uint iSkill3Cool = IDX(vecCool[2].fCoolTime - vecCool[2].fAccTime + 1.f);
    _uint iSkill4Cool = IDX(vecCool[3].fCoolTime - vecCool[3].fAccTime + 1.f);
    _uint iSkill5Cool = IDX(vecCool[4].fCoolTime - vecCool[4].fAccTime + 1.f);

    if (0 < iSkill1Cool && iSkill1Cool < 10 && false == m_bIsCoolChangeOne[0])
        Change_Pos_One_Letter(0);
    if (0 < iSkill2Cool && iSkill2Cool < 10 && false == m_bIsCoolChangeOne[1])
        Change_Pos_One_Letter(1);
    if (0 < iSkill3Cool && iSkill3Cool < 10 && false == m_bIsCoolChangeOne[2])
        Change_Pos_One_Letter(2);
    if (0 < iSkill4Cool && iSkill4Cool < 10 && false == m_bIsCoolChangeOne[3])
        Change_Pos_One_Letter(3);
    if (0 < iSkill5Cool && iSkill5Cool < 10 && false == m_bIsCoolChangeOne[4])
        Change_Pos_One_Letter(4);

    if (0 < iSkill1Cool)
        m_pUi_Skill_Cool[0].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill1Cool);
    if (0 < iSkill2Cool)
        m_pUi_Skill_Cool[1].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill2Cool);
    if (0 < iSkill3Cool)
        m_pUi_Skill_Cool[2].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill3Cool);
    if (0 < iSkill4Cool)
        m_pUi_Skill_Cool[3].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill4Cool);
    if (0 < iSkill5Cool)
        m_pUi_Skill_Cool[4].lock()->Get_FontRenderer()->Get_Text() = to_wstring(iSkill5Cool);

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

void CoolTimeCheckScript::Change_Pos_Two_Letter(_uint iIndex)
{
    if (true == m_pUi_Skill_Cool[iIndex].expired())
        return;

    auto pTransform = m_pUi_Skill_Cool[iIndex].lock()->GetOrAddTransform();
    _float4 vecPos = pTransform->Get_State(Transform_State::POS);

    switch (iIndex)
    {
    case 0:
        vecPos.x = 554;
        break;
    case 1:
        vecPos.x = 685;
        break;
    case 2:
        vecPos.x = 813;
        break;
    case 3:
        vecPos.x = 813;
        break;
    case 4:
        vecPos.x = 685;
        break;
    default:
        return;
    }

    pTransform->Set_State(Transform_State::POS, vecPos);
    
    Change_Text_Zero(iIndex);
}

void CoolTimeCheckScript::Change_Pos_One_Letter(_uint iIndex)
{
    if (true == m_pUi_Skill_Cool[iIndex].expired())
        return;

    auto pTransform = m_pUi_Skill_Cool[iIndex].lock()->GetOrAddTransform();
    _float4 vecPos = pTransform->Get_State(Transform_State::POS);
    m_bIsCoolChangeOne[iIndex] = true;

    switch (iIndex)
    {
    case 0:
        vecPos.x = 565;
        break;
    case 1:
        vecPos.x = 695;
        break;
    case 2:
        vecPos.x = 825;
        break;
    case 3:
        vecPos.x = 825;
        break;
    case 4:
        vecPos.x = 695;
        break;
    default:
        return;
    }

    pTransform->Set_State(Transform_State::POS, vecPos);
}

void CoolTimeCheckScript::Change_Text_Zero(_uint iIndex)
{
    if (true == m_pUi_Skill_Cool[iIndex].expired())
        return;

    m_pUi_Skill_Cool[iIndex].lock()->Get_FontRenderer()->Get_Text() = L"";
    m_bIsCoolChangeOne[iIndex] = false;
}
