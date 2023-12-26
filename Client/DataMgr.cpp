#include "pch.h"
#include "DataMgr.h"

void DataMgr::Initialize()
{
    strTemp = L"";
    tagTemp = {};

    m_CurSetHero.resize(3);
    m_CurSetHero[0] = -1;
    m_CurSetHero[1] = -1;
    m_CurSetHero[2] = -1;

    m_HeroData.resize(IDX(HERO::MAX));
	{

		// PLAYER
		{
			auto& tagData = m_HeroData[IDX(HERO::PLAYER)];

			tagData.Hero = HERO::PLAYER;
			tagData.Element = ElementType::DARK;
			tagData.Star = 4;

			tagData.Skill0Combo = 4;
			tagData.Skill1Combo = 3;
			tagData.Skill2Combo = 0;
			tagData.Skill3Combo = 2;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = -1.f;
			tagData.Skill5Cool = -1.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"근거리";
			tagData.ModelTag = L"Player";
			tagData.WeaponTag = L"Weapon_Player";
			tagData.KeyAttack = L"Skill_Player0";
			tagData.KeyEvade = L"Skill_Player1";
			tagData.KeySkill1 = L"Skill_Player2";
			tagData.KeySkill2 = L"Skill_Player3";
			tagData.KeySkill3 = L"Skill_Player4";
			tagData.KeySkill4 = L"Skill_Player5";
			tagData.KeySkill5 = L"Skill_Player6";
			tagData.KeyChangeCard = L"";
			tagData.KeyDeckMini = L"";
			tagData.KeyDeckSelect = L"";
			tagData.KeyHeroInfo = L"";
			tagData.KeyHeroName = L"";
            tagData.KeyDeckMiniAddBg = L"";
		}

		// ACE3
		{
			auto& tagData = m_HeroData[IDX(HERO::ACE3)];

			tagData.Hero = HERO::ACE3;
			tagData.Element = ElementType::WATER;
			tagData.Star = 4;

			tagData.Skill0Combo = 3;
			tagData.Skill1Combo = 0;
			tagData.Skill2Combo = 2;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = 2.f;
			tagData.Skill5Cool = 2.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"근거리";
			tagData.ModelTag = L"Spear_Ace";
			tagData.WeaponTag = L"Weapon_Spear_Ace";
			tagData.KeyAttack = L"Skill_Ace0";
			tagData.KeyEvade = L"Skill_Ace1";
			tagData.KeySkill1 = L"Skill_Ace2";
			tagData.KeySkill2 = L"Skill_Ace3";
			tagData.KeySkill3 = L"Skill_Ace4";
			tagData.KeySkill4 = L"Skill_Ace5";
			tagData.KeySkill5 = L"Skill_Ace6";
			tagData.KeyChangeCard = L"Card_Ace3";
			tagData.KeyDeckMini = L"Card_Deck_Ace3";
			tagData.KeyDeckSelect = L"Big_Card_Ace3";
			tagData.KeyHeroInfo = L"신극";
			tagData.KeyHeroName = L"에이스";
            tagData.SkillHelp = L"UI_Skill_Help_Ace";
            tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Ace3";
		}

		// KYLE
		{
			auto& tagData = m_HeroData[IDX(HERO::KYLE)];

			tagData.Hero = HERO::KYLE;
			tagData.Element = ElementType::FIRE;
			tagData.Star = 4;

			tagData.Skill0Combo = 4;
			tagData.Skill1Combo = 2;
			tagData.Skill2Combo = 3;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = 2.f;
			tagData.Skill5Cool = 2.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"중거리";
			tagData.ModelTag = L"Kyle";
			tagData.WeaponTag = L"";
			tagData.KeyAttack = L"Skill_Kyle0";
			tagData.KeyEvade = L"Skill_Kyle1";
			tagData.KeySkill1 = L"Skill_Kyle2";
			tagData.KeySkill2 = L"Skill_Kyle3";
			tagData.KeySkill3 = L"Skill_Kyle4";
			tagData.KeySkill4 = L"Skill_Kyle5";
			tagData.KeySkill5 = L"Skill_Kyle6";
			tagData.KeyChangeCard = L"Card_Kyle";
			tagData.KeyDeckMini = L"Card_Deck_Kyle";
			tagData.KeyDeckSelect = L"Big_Card_Kyle";
			tagData.KeyHeroInfo = L"밤의추적자";
			tagData.KeyHeroName = L"카일";
            tagData.SkillHelp = L"UI_Skill_Help_Kyle";
            tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Kyle";
		}

		// YEOPO
		{
			auto& tagData = m_HeroData[IDX(HERO::YEOPO)];

			tagData.Hero = HERO::YEOPO;
			tagData.Element = ElementType::FIRE;
			tagData.Star = 4;

			tagData.Skill0Combo = 3;
			tagData.Skill1Combo = 2;
			tagData.Skill2Combo = 0;
			tagData.Skill3Combo = 4;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = 2.f;
			tagData.Skill5Cool = 2.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"근거리";
			tagData.ModelTag = L"Yeopo";
			tagData.WeaponTag = L"Weapon_Yeopo";
			tagData.KeyAttack = L"Skill_Yeopo0";
			tagData.KeyEvade = L"Skill_Yeopo1";
			tagData.KeySkill1 = L"Skill_Yeopo2";
			tagData.KeySkill2 = L"Skill_Yeopo3";
			tagData.KeySkill3 = L"Skill_Yeopo4";
			tagData.KeySkill4 = L"Skill_Yeopo5";
			tagData.KeySkill5 = L"Skill_Yeopo6";
			tagData.KeyChangeCard = L"Card_Yeopo";
			tagData.KeyDeckMini = L"Card_Deck_Yeopo";
			tagData.KeyDeckSelect = L"Big_Card_Yeopo";
			tagData.KeyHeroInfo = L"무신";
			tagData.KeyHeroName = L"여포";
            tagData.SkillHelp = L"UI_Skill_Help_Yeopo";
            tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Yeopo";
		}

		// DELLONS
		{
			auto& tagData = m_HeroData[IDX(HERO::DELLONS)];

			tagData.Hero = HERO::DELLONS;
			tagData.Element = ElementType::DARK;
			tagData.Star = 4;

			tagData.Skill0Combo = 4;
			tagData.Skill1Combo = 2;
			tagData.Skill2Combo = 2;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = 2.f;
			tagData.Skill5Cool = 2.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"근거리";
			tagData.ModelTag = L"Dellons";
			tagData.WeaponTag = L"Weapon_Dellons";
			tagData.KeyAttack = L"Skill_Dellons0";
			tagData.KeyEvade = L"Skill_Dellons1";
			tagData.KeySkill1 = L"Skill_Dellons2";
			tagData.KeySkill2 = L"Skill_Dellons3";
			tagData.KeySkill3 = L"Skill_Dellons4";
			tagData.KeySkill4 = L"Skill_Dellons5";
			tagData.KeySkill5 = L"Skill_Dellons6";
			tagData.KeyChangeCard = L"Card_Dellons";
			tagData.KeyDeckMini = L"Card_Deck_Dellons";
			tagData.KeyDeckSelect = L"Big_Card_Dellons";
			tagData.KeyHeroInfo = L"죽음의군주";
			tagData.KeyHeroName = L"델론즈";
            tagData.SkillHelp = L"UI_Skill_Help_Dellons";
            tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Dellons";
		}

		// SPIKE
		{
			auto& tagData = m_HeroData[IDX(HERO::SPIKE)];

			tagData.Hero = HERO::SPIKE;
			tagData.Element = ElementType::WATER;
			tagData.Star = 4;

			tagData.Skill0Combo = 4;
			tagData.Skill1Combo = 2;
			tagData.Skill2Combo = 0;
			tagData.Skill3Combo = 0;
			tagData.Skill4Combo = 0;
			tagData.Skill5Combo = 0;

			tagData.Skill1Info = SkillInfo::MOVING;
			tagData.Skill2Info = SkillInfo::CHARGING;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = 2.f;
			tagData.Skill5Cool = 2.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"근거리";
			tagData.ModelTag = L"Spike";
			tagData.WeaponTag = L"Weapon_Spike";
			tagData.KeyAttack = L"Skill_Spike0";
			tagData.KeyEvade = L"Skill_Spike1";
			tagData.KeySkill1 = L"Skill_Spike2";
			tagData.KeySkill2 = L"Skill_Spike3";
			tagData.KeySkill3 = L"Skill_Spike4";
			tagData.KeySkill4 = L"Skill_Spike5";
			tagData.KeySkill5 = L"Skill_Spike6";
			tagData.KeyChangeCard = L"Card_Spike";
			tagData.KeyDeckMini = L"Card_Deck_Spike";
			tagData.KeyDeckSelect = L"Big_Card_Dellons";
			tagData.KeyHeroInfo = L"혹한의폭군";
			tagData.KeyHeroName = L"스파이크";
            tagData.SkillHelp = L"UI_Skill_Help_Spike";
            tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Spike";
		}

		// SHANE
		{
			auto& tagData = m_HeroData[IDX(HERO::SHANE)];

			tagData.Hero = HERO::SHANE;
			tagData.Element = ElementType::DARK;
			tagData.Star = 4;

			tagData.Skill0Combo = 4;
			tagData.Skill1Combo = 2;
			tagData.Skill2Combo = 2;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = 2.f;
			tagData.Skill5Cool = 2.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"근거리";
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
			tagData.KeyHeroInfo = L"악마의화신";
			tagData.KeyHeroName = L"세인";
            tagData.SkillHelp = L"UI_Skill_Help_Shane";
            tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Shane";
		}

		// YEONHEE
		{
			auto& tagData = m_HeroData[IDX(HERO::YEONHEE)];

			tagData.Hero = HERO::YEONHEE;
			tagData.Element = ElementType::LIGHT;
			tagData.Star = 4;

			tagData.Skill0Combo = 3;
			tagData.Skill1Combo = 0;
			tagData.Skill2Combo = 0;
			tagData.Skill3Combo = 0;
			tagData.Skill4Combo = 0;
			tagData.Skill5Combo = 0;

			tagData.Skill1Info = SkillInfo::HOLDING;
			tagData.Skill2Info = SkillInfo::MOVING;
			tagData.Skill3Info = SkillInfo::MOVING;
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

			tagData.Skill1Cool = 2.f;
			tagData.Skill2Cool = 2.f;
			tagData.Skill3Cool = 2.f;
			tagData.Skill4Cool = 2.f;
			tagData.Skill5Cool = 2.f;
			tagData.Skill1DamageMul = 1.f;
			tagData.Skill2DamageMul = 1.f;
			tagData.Skill3DamageMul = 1.f;
			tagData.Skill4DamageMul = 1.f;
			tagData.Skill5DamageMul = 1.f;

			tagData.KeyWeapon = L"원거리";
			tagData.ModelTag = L"Yeonhee";
			tagData.WeaponTag = L"";
			tagData.KeyAttack = L"Skill_Yeonhee0";
			tagData.KeyEvade = L"Skill_Yeonhee1";
			tagData.KeySkill1 = L"Skill_Yeonhee2";
			tagData.KeySkill2 = L"Skill_Yeonhee3";
			tagData.KeySkill3 = L"Skill_Yeonhee4";
			tagData.KeySkill4 = L"Skill_Yeonhee5";
			tagData.KeySkill5 = L"Skill_Yeonhee6";
			tagData.KeyChangeCard = L"Card_Yeonhee";
			tagData.KeyDeckMini = L"Card_Deck_Yeonhee";
			tagData.KeyDeckSelect = L"Big_Card_Yeonhee";
			tagData.KeyHeroInfo = L"종말의인도자";
			tagData.KeyHeroName = L"연희";
            tagData.SkillHelp = L"UI_Skill_Help_Yeonhee";
            tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Yeonhee";
		}
	}


    m_BossData.resize(IDX(BOSS::MAX));
	{

		// MIR
		{
			auto& tagData = m_BossData[IDX(BOSS::MIR)];

			tagData.Element = ElementType::FIRE;
			tagData.Name = L"타락한 화룡";
			tagData.IsUseGroggy = false;
		}

		// AXE
		{
			auto& tagData = m_BossData[IDX(BOSS::AXE)];

			tagData.Element = ElementType::WIND;
			tagData.Name = L"뇌우의 도끼";
			tagData.IsUseGroggy = false;
		}

		// SPIKE
		{
			auto& tagData = m_BossData[IDX(BOSS::SPIKE)];

			tagData.Element = ElementType::WATER;
			tagData.Name = L"스파이크";
			tagData.IsUseGroggy = true;
		}

		// DELLONS
		{
			auto& tagData = m_BossData[IDX(BOSS::DELLONS)];

			tagData.Element = ElementType::DARK;
			tagData.Name = L"델론즈";
			tagData.IsUseGroggy = false;
		}
	}


    {
        m_KeyElement.resize(ElementType::ElementEnd);
        m_KeyElement[IDX(ElementType::DARK)] = L"Element_Dark";
        m_KeyElement[IDX(ElementType::EARTH)] = L"Element_Earth";
        m_KeyElement[IDX(ElementType::FIRE)] = L"Element_Fire";
        m_KeyElement[IDX(ElementType::LIGHT)] = L"Element_Light";
        m_KeyElement[IDX(ElementType::WATER)] = L"Element_Water";
        m_KeyElement[IDX(ElementType::WIND)] = L"Element_Wind";
    }


    {
        m_NpcName.resize(IDX(NPCTYPE::MAX));
        m_NpcName[IDX(NPCTYPE::QUEST)] = L"아리엘";
        m_NpcName[IDX(NPCTYPE::GACHA)] = L"카드 뽑기";
        m_NpcName[IDX(NPCTYPE::POTION)] = L"상점";
        m_NpcName[IDX(NPCTYPE::HIDE_KID)] = L"꼬맹이";
        m_NpcName[IDX(NPCTYPE::SPIKE)] = L"스파이크";
        m_NpcName[IDX(NPCTYPE::SPIKE_AFTER_COMBAT5)] = L"스파이크";
        m_NpcName[IDX(NPCTYPE::DELLONS)] = L"델론즈";
        m_NpcName[IDX(NPCTYPE::DELLONS_AFTER_COMBAT5)] = L"델론즈";
        m_NpcName[IDX(NPCTYPE::SHANE)] = L"세인";
        m_NpcName[IDX(NPCTYPE::PORTAL)] = L"포탈 사용";
        m_NpcName[IDX(NPCTYPE::FENI)] = L"페니";
        m_NpcName[IDX(NPCTYPE::FISHERMAN)] = L"어부";
        m_NpcName[IDX(NPCTYPE::GUARD)] = L"경비병";
    }


    m_DialogAccept.resize(IDX(QUESTINDEX::MAX));
    m_DialogNoClear.resize(IDX(QUESTINDEX::MAX));
    m_DialogClear.resize(IDX(QUESTINDEX::MAX));
    m_Quest.resize(IDX(QUESTINDEX::MAX));

    {
        _uint iIndex = IDX(QUESTINDEX::TRY_GACHA);
        m_DialogAccept[iIndex].push_back(L"가챠퀘0");
        m_DialogAccept[iIndex].push_back(L"가챠퀘1");
        m_DialogAccept[iIndex].push_back(L"가챠퀘2");
        m_DialogAccept[iIndex].push_back(L"가챠퀘3");

        m_DialogNoClear[iIndex].push_back(L"깨고오세요");
        m_DialogNoClear[iIndex].push_back(L"깨고와라");

        m_DialogClear[iIndex].push_back(L"깨고왔구나");
        m_DialogClear[iIndex].push_back(L"테스트");
        m_DialogClear[iIndex].push_back(L"끝");

        m_Quest[iIndex].IsClear = false;
        m_Quest[iIndex].Type = QUESTTYPE::ACT;
        m_Quest[iIndex].CurCount = 0;
        m_Quest[iIndex].MaxCount = 0;
        m_Quest[iIndex].Name = L"가챠 시도";
        m_Quest[iIndex].Info = L"가챠를 하세요";
        m_Quest[iIndex].Clear = L"가챠 끗";
    }

    
    {
        _uint iIndex = IDX(QUESTINDEX::KILL_SPIKE);
        m_DialogAccept[iIndex].push_back(L"마을 밖에 눈이 잔뜩 내린 곳에 가 보았나?");
        m_DialogAccept[iIndex].push_back(L"그 곳에 스파이크란 녀석이 나타나서 싸움을 잘 하는 사람을 \n기다리고 있다더군.");
        m_DialogAccept[iIndex].push_back(L"자신있다면 눈이 잔뜩 내린 그 곳에 가보게.");

        m_DialogNoClear[iIndex].push_back(L"아직 안 갔나?");

        m_Quest[iIndex].IsClear = false;
        m_Quest[iIndex].Type = QUESTTYPE::BOSS;
        m_Quest[iIndex].CurCount = 0;
        m_Quest[iIndex].MaxCount = 0;
        m_Quest[iIndex].Name = L"골목대장 스파이크";
        m_Quest[iIndex].Info = L"눈 내린 곳으로 가 스파이크와 승부를 내자";
        m_Quest[iIndex].Clear = L"이 골목의 대장은 바로 나!";
    }

    
    {
        _uint iIndex = IDX(QUESTINDEX::KILL_DELLONS);
        m_DialogAccept[iIndex].push_back(L"계승자님 저희 마을에 있던 알파카가 사라졌어요 \n도와주실 수 있을까요?");
        m_DialogAccept[iIndex].push_back(L"알파카를 훔쳐간 도둑을 처단하고 \n알파카를 다시 데려와주세요.");

        m_DialogNoClear[iIndex].push_back(L"아직인가요?");

        m_DialogClear[iIndex].push_back(L"알파카가 왜 세마리만 돌아왔나요?");

        m_Quest[iIndex].IsClear = false;
        m_Quest[iIndex].Type = QUESTTYPE::HUNT;
        m_Quest[iIndex].CurCount = 0;
        m_Quest[iIndex].MaxCount = 3;
        m_Quest[iIndex].Name = L"도둑 잡기";
        m_Quest[iIndex].Info = L"알파카를 다시 데려오자";
        m_Quest[iIndex].Clear = L"마을에 돌아가 보고하자";
    }

	{
		_uint iIndex = IDX(QUESTINDEX::HIDE_AND_SEEK);
		m_DialogAccept[iIndex].push_back(L"아저씨, 숨바꼭질 잘해요?");
		m_DialogAccept[iIndex].push_back(L"저를 찾으면 가챠 10회권을 드릴게요");

		m_DialogNoClear[iIndex].push_back(L"아직인가요?");

		m_DialogClear[iIndex].push_back(L"대단하네요!");

		m_Quest[iIndex].IsClear = false;
		m_Quest[iIndex].Type = QUESTTYPE::ACT;
		m_Quest[iIndex].CurCount = 0;
		m_Quest[iIndex].MaxCount = 0;
		m_Quest[iIndex].Name = L"술래 잡기";
		m_Quest[iIndex].Info = L"마을에 숨은 꼬마를 찾자.";
		m_Quest[iIndex].Clear = L"꼬마에게 보고하자";
	}

	{
		_uint iIndex = IDX(QUESTINDEX::KILL_MIR);
		m_DialogAccept[iIndex].push_back(L"미르");
		m_DialogAccept[iIndex].push_back(L"미르");
		m_DialogAccept[iIndex].push_back(L"미르");

		m_DialogNoClear[iIndex].push_back(L"미르");

		m_Quest[iIndex].IsClear = false;
		m_Quest[iIndex].Type = QUESTTYPE::BOSS;
		m_Quest[iIndex].CurCount = 0;
		m_Quest[iIndex].MaxCount = 0;
		m_Quest[iIndex].Name = L"미르";
		m_Quest[iIndex].Info = L"미르";
		m_Quest[iIndex].Clear = L"미르";
	}

    m_BossDialog.resize(IDX(BOSS::MAX));
    {
        auto& Dialog = m_BossDialog[IDX(BOSS::DELLONS)];
        
        Dialog.push_back(L"나는 어둠의 다크 델론즈");
        Dialog.push_back(L"알파카를 찾으러 왔나? \n어쩌지, 세 마리 빼고는 이미 나의 부하가 되었다.");
        Dialog.push_back(L"남은 알파카라도 데려가려면 날 이겨봐라.");
    }



    m_NoQuestDialog.resize(IDX(QUESTINDEX::MAX));
    {
        m_NoQuestDialog[IDX(QUESTINDEX::TRY_GACHA)]     = L"... ...";
        m_NoQuestDialog[IDX(QUESTINDEX::KILL_ALPACA)]   = L"... ...";
        m_NoQuestDialog[IDX(QUESTINDEX::KILL_DELLONS)]  = L"알파카 돌려줘요...";
        m_NoQuestDialog[IDX(QUESTINDEX::KILL_SPIKE)]    = L"... ...";
        m_NoQuestDialog[IDX(QUESTINDEX::KILL_MIR)]      = L"... ...";
        m_NoQuestDialog[IDX(QUESTINDEX::STORY_CLEAR)]   = L"... ...";
        m_NoQuestDialog[IDX(QUESTINDEX::HIDE_AND_SEEK)]   = L"... ...";
    }


    

    m_NoAcceptDialog.resize(IDX(QUESTINDEX::MAX));
    {
        m_NoAcceptDialog[IDX(QUESTINDEX::TRY_GACHA)]     = L"바쁘시군요";
        m_NoAcceptDialog[IDX(QUESTINDEX::KILL_ALPACA)]   = L"바쁘시군요";
        m_NoAcceptDialog[IDX(QUESTINDEX::KILL_DELLONS)]  = L"바쁘시군요";
        m_NoAcceptDialog[IDX(QUESTINDEX::KILL_SPIKE)]    = L"바쁘시군요";
        m_NoAcceptDialog[IDX(QUESTINDEX::KILL_MIR)]      = L"바쁘시군요";
        m_NoAcceptDialog[IDX(QUESTINDEX::STORY_CLEAR)]   = L"바쁘시군요";
        m_NoAcceptDialog[IDX(QUESTINDEX::HIDE_AND_SEEK)]   = L"바쁘시군요";
    }




    _uint iSize = IDX(DUMMY_HERO::MAX);
    m_DummyData.resize(iSize);
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& tagData = m_DummyData[i];
        wstring strIndex = to_wstring(i);
        tagData.KeyDeckMini     = L"Card_Deck_Dummy" + strIndex;
        tagData.KeyDeckSelect   = L"Bick_Card_Dummy" + strIndex;
        tagData.KeyDeckMiniAddBg = L"Card_Deck_Add_Bg_Dummy" + strIndex;
    }

    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_ARIA)];

        tagData.Element     = ElementType::WATER;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"용제후계자";
        tagData.KeyHeroName = L"아리아";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_BANE)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"야망의반역자";
        tagData.KeyHeroName = L"베인";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_ENT)];

        tagData.Element     = ElementType::EARTH;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"녹님의거목";
        tagData.KeyHeroName = L"천목";
    }
     
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_GOBLIN)];

        tagData.Element     = ElementType::EARTH;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"숲의악몽";
        tagData.KeyHeroName = L"고블린";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_ONI)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"백전노장";
        tagData.KeyHeroName = L"오니";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_PANDA)];

        tagData.Element     = ElementType::EARTH;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"취권의대가";
        tagData.KeyHeroName = L"판다";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_NAGA)];

        tagData.Element     = ElementType::WATER;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"표독한저주";
        tagData.KeyHeroName = L"나가여왕";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_SCOTT)];

        tagData.Element     = ElementType::WIND;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"거침없는질주";
        tagData.KeyHeroName = L"오대오";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_VAMPIRE)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"피의군주";
        tagData.KeyHeroName = L"뱀파이어로드";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_WEREWOLF)];

        tagData.Element     = ElementType::EARTH;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"야성의발톱";
        tagData.KeyHeroName = L"웨어울프";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_BRANSEL)];

        tagData.Element     = ElementType::LIGHT;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"순백의기사";
        tagData.KeyHeroName = L"브란셀";
    }
      
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_DARKARMY_AXE)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 1;
        tagData.KeyHeroInfo = L"어둠의군세";
        tagData.KeyHeroName = L"도끼병";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_DARKARMY_CROSSBOW)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 1;
        tagData.KeyHeroInfo = L"어둠의군세";
        tagData.KeyHeroName = L"궁병";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_DARKARMY_SWORD)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 1;
        tagData.KeyHeroInfo = L"어둠의군세";
        tagData.KeyHeroName = L"검병";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_DEA)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"희망의하모니";
        tagData.KeyHeroName = L"데아";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_FERUS)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"전장의투사";
        tagData.KeyHeroName = L"페루스";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_GEHENNA)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"언령의인형술사";
        tagData.KeyHeroName = L"게헨나";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_HAERI)];

        tagData.Element     = ElementType::WATER;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"물의영수";
        tagData.KeyHeroName = L"해리";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_JUNO)];

        tagData.Element     = ElementType::WIND;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"삼족오의수장";
        tagData.KeyHeroName = L"준오";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_KAGURA)];

        tagData.Element     = ElementType::WIND;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"삼판의무녀";
        tagData.KeyHeroName = L"카구라";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_KARIN)];

        tagData.Element     = ElementType::EARTH;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"불멸의성녀";
        tagData.KeyHeroName = L"카린";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_KARMA)];

        tagData.Element     = ElementType::EARTH;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"절망의신선";
        tagData.KeyHeroName = L"카르마";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_LINA)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"고요한선율";
        tagData.KeyHeroName = L"리나";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_LUCIA)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"고양이마법사";
        tagData.KeyHeroName = L"루시아";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_MELYN)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"흑마법연구가";
        tagData.KeyHeroName = L"멜린";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_MERCURE)];

        tagData.Element     = ElementType::WATER;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"광기의연구자";
        tagData.KeyHeroName = L"멜키르";
    }

    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_WATT)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"건설감독관";
        tagData.KeyHeroName = L"와트";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_OSCAR)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"광기의칼날";
        tagData.KeyHeroName = L"오스카";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_PALLANUS)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"기회의지휘관";
        tagData.KeyHeroName = L"팔라누스";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_PASCAL)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"황제";
        tagData.KeyHeroName = L"파스칼";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_PHYSYS1)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 1;
        tagData.KeyHeroInfo = L"피지스";
        tagData.KeyHeroName = L"어둠의검사";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_PHYSYS2)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 1;
        tagData.KeyHeroInfo = L"피지스";
        tagData.KeyHeroName = L"어둠의사제";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_PLANE)];

        tagData.Element     = ElementType::WATER;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"열혈매니저";
        tagData.KeyHeroName = L"플라네";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_PLATON)];

        tagData.Element     = ElementType::WIND;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"여신의사자";
        tagData.KeyHeroName = L"플라톤";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_RACHEL1)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"불멸의화신";
        tagData.KeyHeroName = L"레이첼";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_RACHEL2)];

        tagData.Element     = ElementType::FIRE;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"워커홀릭";
        tagData.KeyHeroName = L"레이첼";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_RADIN)];

        tagData.Element     = ElementType::WATER;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"검은마법사";
        tagData.KeyHeroName = L"라딘";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_RADISAND)];

        tagData.Element     = ElementType::WATER;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"은빛그림자";
        tagData.KeyHeroName = L"레디샌즈";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_SILVESTA)];

        tagData.Element     = ElementType::EARTH;
        tagData.Star        = 2;
        tagData.KeyHeroInfo = L"검은정의";
        tagData.KeyHeroName = L"실베스타";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_SUCCUBUS)];

        tagData.Element     = ElementType::DARK;
        tagData.Star        = 1;
        tagData.KeyHeroInfo = L"어둠의악몽";
        tagData.KeyHeroName = L"서큐버스";
    }
    
    {
        auto& tagData = m_DummyData[IDX(DUMMY_HERO::DUMMY_VANESSA)];

        tagData.Element     = ElementType::WIND;
        tagData.Star        = 3;
        tagData.KeyHeroInfo = L"운명의지휘자";
        tagData.KeyHeroName = L"바네사";
    }


    m_CardInven.resize(100);
    Set_Card_Inven(HERO::YEONHEE);

    m_NpcDialog.resize(IDX(NPCTYPE::MAX));
    
    //for (_uint i = 0; i < IDX(NPCTYPE::MAX); ++i)
    //    m_NpcDialog[i].resize(3);

    m_NpcDialog[IDX(NPCTYPE::SPIKE)].resize(1);
    m_NpcDialog[IDX(NPCTYPE::DELLONS)].resize(1);
    m_NpcDialog[IDX(NPCTYPE::DELLONS_AFTER_COMBAT5)].resize(1);
    m_NpcDialog[IDX(NPCTYPE::SHANE)].resize(1);
    m_NpcDialog[IDX(NPCTYPE::FENI)].resize(1);
    m_NpcDialog[IDX(NPCTYPE::FISHERMAN)].resize(2);

    m_NpcDialog[IDX(NPCTYPE::SPIKE)][0].push_back(L"안녕 나는 스파이크야.");
    m_NpcDialog[IDX(NPCTYPE::SPIKE)][0].push_back(L"눈썰매장을 찾고 있는데 혹시 나에게 길을 안내해주지 않겠어?");
    m_NpcDialog[IDX(NPCTYPE::SPIKE)][0].push_back(L"뭐? 싫다고?! 그래도 같이 따라갈거야. 얼른 안내해. 어쩌라고. 뭐.");
    
    //m_NpcDialog[IDX(NPCTYPE::SPIKE)][1].push_back(L"이건 테스트 1-0");
    //m_NpcDialog[IDX(NPCTYPE::SPIKE)][1].push_back(L"이건 테스트 1-1");
    //m_NpcDialog[IDX(NPCTYPE::SPIKE)][1].push_back(L"이건 테스트 1-2");
    //
    //m_NpcDialog[IDX(NPCTYPE::SPIKE)][2].push_back(L"이건 테스트 2-0");
    //m_NpcDialog[IDX(NPCTYPE::SPIKE)][2].push_back(L"이건 테스트 2-1");
    //m_NpcDialog[IDX(NPCTYPE::SPIKE)][2].push_back(L"이건 테스트 2-2");

    m_NpcDialog[IDX(NPCTYPE::DELLONS)][0].push_back(L"크크크킄 나는 어둠의 다크 델론즈.");
    m_NpcDialog[IDX(NPCTYPE::DELLONS)][0].push_back(L"너 좀 약해보이는군 이 앞은 약해빠진 너에게는 많이 위험하지.");
    m_NpcDialog[IDX(NPCTYPE::DELLONS)][0].push_back(L"그러므로 이 몸께서 친히 널 도와주도록 하겠다");
    
	m_NpcDialog[IDX(NPCTYPE::DELLONS_AFTER_COMBAT5)][0].push_back(L"Dell Test");
	m_NpcDialog[IDX(NPCTYPE::DELLONS_AFTER_COMBAT5)][0].push_back(L"Dell Test");
	m_NpcDialog[IDX(NPCTYPE::DELLONS_AFTER_COMBAT5)][0].push_back(L"Dell Test");


    m_NpcDialog[IDX(NPCTYPE::SHANE)][0].push_back(L"안녕 나는 올해로 19세인 세인이야. 혹시 함께 할 동료를 찾고 있어?");
    m_NpcDialog[IDX(NPCTYPE::SHANE)][0].push_back(L"바로 알겠다고? 너 얼빠구나.");
    m_NpcDialog[IDX(NPCTYPE::SHANE)][0].push_back(L"알겠어 특별히 같이 가주지.");

	m_NpcDialog[IDX(NPCTYPE::FENI)][0].push_back(L"구해주셔서 감사해요. 하마터면 죽을 뻔 했어요");
	m_NpcDialog[IDX(NPCTYPE::FENI)][0].push_back(L"저는 페니라고 해요. 전 이만 마을로 가봐야겠어요!");
	m_NpcDialog[IDX(NPCTYPE::FENI)][0].push_back(L"구해주신 보답으로 돈을 조금 드릴게요.");

	m_NpcDialog[IDX(NPCTYPE::FISHERMAN)][0].push_back(L"우리 아지트를 구해줘서 고맙네!");
	m_NpcDialog[IDX(NPCTYPE::FISHERMAN)][0].push_back(L"우리가 보기엔 우락부락하지만 싸움을 못해.");

	m_NpcDialog[IDX(NPCTYPE::FISHERMAN)][1].push_back(L"이제 다시 고기를 잡을 수 있겠군!");
	m_NpcDialog[IDX(NPCTYPE::FISHERMAN)][1].push_back(L"고맙네.");



}

_uint DataMgr::Get_Dialog_Size(QUESTINDEX eIndex, CUR_QUEST eType)
{
    if (QUESTINDEX::MAX == eIndex)
        return 0;

    _uint iQuestIndex = IDX(eIndex);
    if (CUR_QUEST::ACCEPT == eType)
        return IDX(m_DialogAccept[iQuestIndex].size());

    else if (CUR_QUEST::PROGRESS == eType)
        return IDX(m_DialogNoClear[iQuestIndex].size());

    else if(CUR_QUEST::CLEAR == eType)
        return IDX(m_DialogClear[iQuestIndex].size());

    return 0;
}

const wstring& DataMgr::Get_Dialog(QUESTINDEX eIndex, CUR_QUEST eType, _uint iIndex)
{
    if (QUESTINDEX::MAX == eIndex)
        return strTemp;

    _uint iQuestIndex = IDX(eIndex);
    if (CUR_QUEST::ACCEPT == eType)
        return m_DialogAccept[iQuestIndex][iIndex];

    else if (CUR_QUEST::PROGRESS == eType)
        return m_DialogNoClear[iQuestIndex][iIndex];

    else if (CUR_QUEST::CLEAR == eType)
        return m_DialogClear[iQuestIndex][iIndex];

    return strTemp;
}

_uint DataMgr::Get_Card_Inven_Use_Size()
{
    _uint iSize = IDX(m_CardInven.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_CardInven[i].first)
            return i;
    }

    return iSize;
}

HERODATA& DataMgr::Get_Card_Inven(_uint iIndex)
{
    if (IDX(m_CardInven.size()) <= iIndex)
        return tagTemp;

    if(false == m_CardInven[iIndex].first)
        return tagTemp;

    return m_CardInven[iIndex].second;
}

void DataMgr::Set_Card_Inven(HERO eType)
{
    _uint iSize = IDX(m_CardInven.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& tagData = m_CardInven[i];
        if (false == tagData.first)
        {
            tagData.first = true;
            tagData.second = m_HeroData[IDX(eType)];

            return;
        }
    }
}

void DataMgr::Set_Card_Inven(DUMMY_HERO eType)
{
    _uint iSize = IDX(m_CardInven.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& tagData = m_CardInven[i];
        if (false == tagData.first)
        {
            tagData.first = true;
            tagData.second = m_DummyData[IDX(eType)];

            return;
        }
    }
}

void DataMgr::Remove_Card_Inven(_uint iIndex)
{
    if (IDX(m_CardInven.size()) <= iIndex)
        return;

    auto& tagData = m_CardInven[iIndex];
    if (true == tagData.first)
    {
        tagData.first = false;
        tagData.second = HERODATA();
    }
}

HERO DataMgr::Get_Cur_Set_Hero(_uint iIndex)
{
    if (-1 == m_CurSetHero[iIndex])
        return HERO::MAX;
 
    return m_CardInven[m_CurSetHero[iIndex]].second.Hero;
}

_bool DataMgr::Set_Cur_Hero(_uint iIndex, _uint iCardIndex)
{
    _uint iSize = IDX(m_CardInven.size());
    if (iSize <= iCardIndex)
        return false;

    for (_uint i = 0; i < 3; ++i)
    {
        if (iIndex != i)
            if (m_CurSetHero[i] == iCardIndex)
                return false;
    }

    m_CurSetHero[iIndex] = iCardIndex;
    return true;
}

void DataMgr::Remove_Cur_Hero(_uint iIndex)
{
    if (IDX(m_CurSetHero.size()) <= iIndex)
        return;

    m_CurSetHero[iIndex] = -1;
}

_bool SortByStar(const pair<_bool, HERODATA>& a, const pair<_bool, HERODATA>& b)
{
    return a.second.Star > b.second.Star;
}

void DataMgr::Sort_By_Star()
{
    sort(m_CardInven.begin(), m_CardInven.end(), SortByStar);
}

_uint DataMgr::Get_NpcDialog_Count(NPCTYPE eType)
{
    int a = 0;
     return IDX(m_NpcDialog[IDX(eType)].size()); 
}

