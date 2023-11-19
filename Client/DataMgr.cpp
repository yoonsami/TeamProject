#include "pch.h"
#include "DataMgr.h"

void DataMgr::Initialize()
{
    m_HeroData.resize(IDX(HERO::MAX));

    // PLAYER
    {
        auto& tagData = m_HeroData[IDX(HERO::PLAYER)];
        
        tagData.Element         = ElementType::WATER;
        tagData.Star            = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;

        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

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
        tagData.Skill4Cool      = -1.f;
        tagData.Skill5Cool      = -1.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.ModelTag        = L"Player";
        tagData.WeaponTag       = L"Weapon_Player";
        tagData.KeyAttack       = L"Skill_Player0";
        tagData.KeyEvade        = L"Skill_Player1";
        tagData.KeySkill1       = L"Skill_Player2";
        tagData.KeySkill2       = L"Skill_Player3";
        tagData.KeySkill3       = L"Skill_Player4";
        tagData.KeySkill4       = L"Skill_Player5";
        tagData.KeySkill5       = L"Skill_Player6";
        tagData.KeyChangeCard   = L"";
        tagData.KeyDeckMini     = L"";
        tagData.KeyDeckSelect   = L"";
        
    }

    // ACE3
    {
        auto& tagData = m_HeroData[IDX(HERO::ACE3)];
        
        tagData.Element         = ElementType::WATER;
        tagData.Star            = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;
        
        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

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
        tagData.Skill5Cool      = 15.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.ModelTag        = L"Spear_Ace";
        tagData.WeaponTag       = L"Weapon_Spear_Ace";
        tagData.KeyAttack       = L"Skill_Ace0";
        tagData.KeyEvade        = L"Skill_Ace1";
        tagData.KeySkill1       = L"Skill_Ace2";
        tagData.KeySkill2       = L"Skill_Ace3";
        tagData.KeySkill3       = L"Skill_Ace4";
        tagData.KeySkill4       = L"Skill_Ace5";
        tagData.KeySkill5       = L"Skill_Ace6";
        tagData.KeyChangeCard   = L"Card_Ace3";
        tagData.KeyDeckMini     = L"Card_Deck_Ace3";
        tagData.KeyDeckSelect   = L"Big_Card_Ace3";
        
    }
    
    // KYLE
    {
        auto& tagData = m_HeroData[IDX(HERO::KYLE)];
        
        tagData.Element         = ElementType::FIRE;
        tagData.Star            = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;

        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

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

        tagData.ModelTag        = L"Kyle";
        tagData.WeaponTag       = L"";
        tagData.KeyAttack       = L"Skill_Kyle0";
        tagData.KeyEvade        = L"Skill_Kyle1";
        tagData.KeySkill1       = L"Skill_Kyle2";
        tagData.KeySkill2       = L"Skill_Kyle3";
        tagData.KeySkill3       = L"Skill_Kyle4";
        tagData.KeySkill4       = L"Skill_Kyle5";
        tagData.KeySkill5       = L"Skill_Kyle6";
        tagData.KeyChangeCard   = L"Card_Kyle";
        tagData.KeyDeckMini     = L"Card_Deck_Kyle";
        tagData.KeyDeckSelect   = L"Big_Card_Kyle";
    }

    // YEOPO
    {
        auto& tagData = m_HeroData[IDX(HERO::YEOPO)];
        
        tagData.Element         = ElementType::FIRE;
        tagData.Star            = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;

        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

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

        tagData.ModelTag        = L"Yeopo";
        tagData.WeaponTag       = L"Weapon_Yeopo";
        tagData.KeyAttack       = L"Skill_Yeopo0";
        tagData.KeyEvade        = L"Skill_Yeopo1";
        tagData.KeySkill1       = L"Skill_Yeopo2";
        tagData.KeySkill2       = L"Skill_Yeopo3";
        tagData.KeySkill3       = L"Skill_Yeopo4";
        tagData.KeySkill4       = L"Skill_Yeopo5";
        tagData.KeySkill5       = L"Skill_Yeopo6";
        tagData.KeyChangeCard   = L"Card_Yeopo";
        tagData.KeyDeckMini     = L"Card_Deck_Yeopo";
        tagData.KeyDeckSelect   = L"Big_Card_Yeopo";
    }
    
    // DELLONS
    {
        auto& tagData = m_HeroData[IDX(HERO::DELLONS)];
        
        tagData.Element         = ElementType::DARK;
        tagData.Star            = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;

        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

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

        tagData.ModelTag        = L"Dellons";
        tagData.WeaponTag       = L"Weapon_Dellons";
        tagData.KeyAttack       = L"Skill_Dellons0";
        tagData.KeyEvade        = L"Skill_Dellons1";
        tagData.KeySkill1       = L"Skill_Dellons2";
        tagData.KeySkill2       = L"Skill_Dellons3";
        tagData.KeySkill3       = L"Skill_Dellons4";
        tagData.KeySkill4       = L"Skill_Dellons5";
        tagData.KeySkill5       = L"Skill_Dellons6";
        tagData.KeyChangeCard   = L"Card_Dellons";
        tagData.KeyDeckMini     = L"Card_Deck_Dellons";
        tagData.KeyDeckSelect   = L"Big_Card_Dellons";
    }
    
    // SPIKE
    {
        auto& tagData = m_HeroData[IDX(HERO::SPIKE)];
        
        tagData.Element         = ElementType::WATER;
        tagData.Star            = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;

        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

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

        tagData.ModelTag        = L"Spike";
        tagData.WeaponTag       = L"Weapon_Spike";
        tagData.KeyAttack       = L"Skill_Spike0";
        tagData.KeyEvade        = L"Skill_Spike1";
        tagData.KeySkill1       = L"Skill_Spike2";
        tagData.KeySkill2       = L"Skill_Spike3";
        tagData.KeySkill3       = L"Skill_Spike4";
        tagData.KeySkill4       = L"Skill_Spike5";
        tagData.KeySkill5       = L"Skill_Spike6";
        tagData.KeyChangeCard   = L"Card_Spike";
        tagData.KeyDeckMini     = L"Card_Deck_Spike";
        tagData.KeyDeckSelect   = L"Big_Card_Dellons";
    }

    // RACHEL
    {
        auto& tagData = m_HeroData[IDX(HERO::RACHEL)];

        tagData.Element = ElementType::FIRE;
        tagData.Star = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;

        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

        tagData.AttackDamage = 1.f;
        tagData.MaxHp = 10.f;
        tagData.CurHp = 10.f;
        tagData.PreHp = 10.f;
        tagData.Deffence = 1.f;
        tagData.HpRegen = 1.f;
        tagData.CriPercent = 0.f;
        tagData.CriDamageMul = 1.f;

        tagData.Skill1Cool = 5.f;
        tagData.Skill2Cool = 5.f;
        tagData.Skill3Cool = 5.f;
        tagData.Skill4Cool = 5.f;
        tagData.Skill5Cool = 5.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.ModelTag = L"Rachel";
        tagData.WeaponTag = L"Weapon_Rachel";
        tagData.KeyAttack = L"Skill_Rachel0";
        tagData.KeyEvade = L"Skill_Rachel1";
        tagData.KeySkill1 = L"Skill_Rachel2";
        tagData.KeySkill2 = L"Skill_Rachel3";
        tagData.KeySkill3 = L"Skill_Rachel4";
        tagData.KeySkill4 = L"Skill_Rachel5";
        tagData.KeySkill5 = L"Skill_Rachel6";
        tagData.KeyChangeCard = L"Card_Rachel";
        tagData.KeyDeckMini = L"Card_Deck_Rachel";
        tagData.KeyDeckSelect = L"Big_Card_Rachel";
    }
    

    // SHANE
    {
        auto& tagData = m_HeroData[IDX(HERO::SHANE)];

        tagData.Element = ElementType::DARK;
        tagData.Star = 4;

        tagData.Skill1Combo = 0;
        tagData.Skill2Combo = 0;
        tagData.Skill3Combo = 0;
        tagData.Skill4Combo = 0;
        tagData.Skill5Combo = 0;

        tagData.Skill1Info = SkillInfo::NONE;
        tagData.Skill2Info = SkillInfo::NONE;
        tagData.Skill3Info = SkillInfo::NONE;
        tagData.Skill4Info = SkillInfo::NONE;
        tagData.Skill5Info = SkillInfo::NONE;

        tagData.AttackDamage = 1.f;
        tagData.MaxHp = 10.f;
        tagData.CurHp = 10.f;
        tagData.PreHp = 10.f;
        tagData.Deffence = 1.f;
        tagData.HpRegen = 1.f;
        tagData.CriPercent = 0.f;
        tagData.CriDamageMul = 1.f;

        tagData.Skill1Cool = 5.f;
        tagData.Skill2Cool = 5.f;
        tagData.Skill3Cool = 5.f;
        tagData.Skill4Cool = 5.f;
        tagData.Skill5Cool = 5.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

        tagData.ModelTag = L"Shane";
        tagData.WeaponTag = L"Weapon_Shane";
        tagData.KeyAttack = L"Skill_Shane0";
        tagData.KeyEvade = L"Skill_Shane1";
        tagData.KeySkill1 = L"Skill_Shane2";
        tagData.KeySkill2 = L"Skill_Shane3";
        tagData.KeySkill3 = L"Skill_Shane4";
        tagData.KeySkill4 = L"Skill_Shane5";
        tagData.KeySkill5 = L"Skill_Shane6";
        tagData.KeyChangeCard = L"Card_Shane";
        tagData.KeyDeckMini = L"Card_Deck_Shane";
        tagData.KeyDeckSelect = L"Big_Card_Shane";
    }



    m_KeyElement.resize(ElementType::ElementEnd);
    m_KeyElement[0] = L"Element_Dark";
    m_KeyElement[1] = L"Element_Earth";
    m_KeyElement[2] = L"Element_Fire";
    m_KeyElement[3] = L"Element_Light";
    m_KeyElement[4] = L"Element_Water";
    m_KeyElement[5] = L"Element_Wind";
}
