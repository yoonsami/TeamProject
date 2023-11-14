#include "pch.h"
#include "DataMgr.h"

void DataMgr::Initialize()
{
    m_HeroData.resize(IDX(HERO::MAX));

    // ACE3
    {
        auto& tagData = m_HeroData[IDX(HERO::ACE3)];
        
        tagData.Element         = ElementType::WATER;
        tagData.Star            = 4;

        tagData.AttackDamage    = 1.f;
        tagData.MaxHp           = 10.f;
        tagData.CurHp           = 10.f;
        tagData.PreHp           = 10.f;
        tagData.Deffence        = 1.f;
        tagData.HpRegen         = 1.f;
        tagData.CriPercent      = 0.f;
        tagData.CriDamageMul    = 1.f;

        tagData.Skill1Cool      = 5.f;
        tagData.Skill2Cool      = 5.f;
        tagData.Skill3Cool      = 5.f;
        tagData.Skill4Cool      = 5.f;
        tagData.Skill5Cool      = 5.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.KeyAttack       = L"Skill_Ace0";
        tagData.KeyEvade        = L"Skill_Ace1";
        tagData.KeySkill1       = L"Skill_Ace2";
        tagData.KeySkill2       = L"Skill_Ace3";
        tagData.KeySkill3       = L"Skill_Ace4";
        tagData.KeySkill4       = L"Skill_Ace5";
        tagData.KeySkill5       = L"Skill_Ace6";
        tagData.KeyChangeCard   = L"Card_Ace3";
        tagData.KeyDeckMini     = L"Card_Deck_Ace3";
        tagData.KeyDeckSelect   = L"Big_Card_Ace_Line3";
        
    }
    
    // KYLE
    {
        auto& tagData = m_HeroData[IDX(HERO::KYLE)];
        
        tagData.Element         = ElementType::FIRE;
        tagData.Star            = 4;

        tagData.AttackDamage    = 1.f;
        tagData.MaxHp           = 10.f;
        tagData.CurHp           = 10.f;
        tagData.PreHp           = 10.f;
        tagData.Deffence        = 1.f;
        tagData.HpRegen         = 1.f;
        tagData.CriPercent      = 0.f;
        tagData.CriDamageMul    = 1.f;

        tagData.Skill1Cool      = 5.f;
        tagData.Skill2Cool      = 5.f;
        tagData.Skill3Cool      = 5.f;
        tagData.Skill4Cool      = 5.f;
        tagData.Skill5Cool      = 5.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.KeyAttack       = L"Skill_Kyle0";
        tagData.KeyEvade        = L"Skill_Kyle1";
        tagData.KeySkill1       = L"Skill_Kyle2";
        tagData.KeySkill2       = L"Skill_Kyle3";
        tagData.KeySkill3       = L"Skill_Kyle4";
        tagData.KeySkill4       = L"Skill_Kyle5";
        tagData.KeySkill5       = L"Skill_Kyle6";
        tagData.KeyChangeCard   = L"Card_Kyle";
        tagData.KeyDeckMini     = L"Card_Deck_Kyle";
        tagData.KeyDeckSelect   = L"Big_Card_Kyle_Line";
    }

    // YEOPO
    {
        auto& tagData = m_HeroData[IDX(HERO::YEOPO)];
        
        tagData.Element         = ElementType::FIRE;
        tagData.Star            = 4;

        tagData.AttackDamage    = 1.f;
        tagData.MaxHp           = 10.f;
        tagData.CurHp           = 10.f;
        tagData.PreHp           = 10.f;
        tagData.Deffence        = 1.f;
        tagData.HpRegen         = 1.f;
        tagData.CriPercent      = 0.f;
        tagData.CriDamageMul    = 1.f;

        tagData.Skill1Cool      = 5.f;
        tagData.Skill2Cool      = 5.f;
        tagData.Skill3Cool      = 5.f;
        tagData.Skill4Cool      = 5.f;
        tagData.Skill5Cool      = 5.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.KeyAttack       = L"Skill_Yeopo0";
        tagData.KeyEvade        = L"Skill_Yeopo1";
        tagData.KeySkill1       = L"Skill_Yeopo2";
        tagData.KeySkill2       = L"Skill_Yeopo3";
        tagData.KeySkill3       = L"Skill_Yeopo4";
        tagData.KeySkill4       = L"Skill_Yeopo5";
        tagData.KeySkill5       = L"Skill_Yeopo6";
        tagData.KeyChangeCard   = L"Card_Yeopo";
        tagData.KeyDeckMini     = L"Card_Deck_Yeopo";
        tagData.KeyDeckSelect   = L"Big_Card_Yeopo_Line";
    }
    
    // DELLONS
    {
        auto& tagData = m_HeroData[IDX(HERO::DELLONS)];
        
        tagData.Element         = ElementType::DARK;
        tagData.Star            = 4;

        tagData.AttackDamage    = 1.f;
        tagData.MaxHp           = 10.f;
        tagData.CurHp           = 10.f;
        tagData.PreHp           = 10.f;
        tagData.Deffence        = 1.f;
        tagData.HpRegen         = 1.f;
        tagData.CriPercent      = 0.f;
        tagData.CriDamageMul    = 1.f;

        tagData.Skill1Cool      = 5.f;
        tagData.Skill2Cool      = 5.f;
        tagData.Skill3Cool      = 5.f;
        tagData.Skill4Cool      = 5.f;
        tagData.Skill5Cool      = 5.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.KeyAttack       = L"Skill_Dellons0";
        tagData.KeyEvade        = L"Skill_Dellons1";
        tagData.KeySkill1       = L"Skill_Dellons2";
        tagData.KeySkill2       = L"Skill_Dellons3";
        tagData.KeySkill3       = L"Skill_Dellons4";
        tagData.KeySkill4       = L"Skill_Dellons5";
        tagData.KeySkill5       = L"Skill_Dellons6";
        tagData.KeyChangeCard   = L"Card_Dellons";
        tagData.KeyDeckMini     = L"Card_Deck_Dellons";
        tagData.KeyDeckSelect   = L"Big_Card_Dellons_Line";
    }
    

    m_KeyElement.resize(ElementType::ElementEnd);
    m_KeyElement[0] = L"Element_Dark";
    m_KeyElement[1] = L"Element_Earth";
    m_KeyElement[2] = L"Element_Fire";
    m_KeyElement[3] = L"Element_Light";
    m_KeyElement[4] = L"Element_Water";
    m_KeyElement[5] = L"Element_Wind";
}
