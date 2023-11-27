#include "pch.h"
#include "DataMgr.h"

void DataMgr::Initialize()
{
    strTemp = L"";

    m_HeroData.resize(IDX(HERO::MAX));

    // PLAYER
    {
        auto& tagData = m_HeroData[IDX(HERO::PLAYER)];

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

        tagData.Skill1Cool = 5.f;
        tagData.Skill2Cool = 5.f;
        tagData.Skill3Cool = 5.f;
        tagData.Skill4Cool = -1.f;
        tagData.Skill5Cool = -1.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

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

    }

    // ACE3
    {
        auto& tagData = m_HeroData[IDX(HERO::ACE3)];

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

        tagData.Skill1Cool = 5.f;
        tagData.Skill2Cool = 5.f;
        tagData.Skill3Cool = 5.f;
        tagData.Skill4Cool = 5.f;
        tagData.Skill5Cool = 15.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

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

    }

    // KYLE
    {
        auto& tagData = m_HeroData[IDX(HERO::KYLE)];

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

    }

    // YEOPO
    {
        auto& tagData = m_HeroData[IDX(HERO::YEOPO)];

        tagData.Element = ElementType::FIRE;
        tagData.Star = 4;

        tagData.Skill0Combo = 4;
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

        tagData.Skill1Cool = 35.f;
        tagData.Skill2Cool = 35.f;
        tagData.Skill3Cool = 35.f;
        tagData.Skill4Cool = 3.f;
        tagData.Skill5Cool = 35.f;
        tagData.Skill1DamageMul = 1.f;
        tagData.Skill2DamageMul = 1.f;
        tagData.Skill3DamageMul = 1.f;
        tagData.Skill4DamageMul = 1.f;
        tagData.Skill5DamageMul = 1.f;

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

    }

    // DELLONS
    {
        auto& tagData = m_HeroData[IDX(HERO::DELLONS)];

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

    }

    // SPIKE
    {
        auto& tagData = m_HeroData[IDX(HERO::SPIKE)];

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

    }

    // SHANE
    {
        auto& tagData = m_HeroData[IDX(HERO::SHANE)];

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
        tagData.KeyHeroInfo = L"악마의화신";
        tagData.KeyHeroName = L"세인";

    }

    // YEONHEE
    {
        auto& tagData = m_HeroData[IDX(HERO::YEONHEE)];

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

    }


    m_BossData.resize(IDX(BOSS::MAX));

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


    {
        m_KeyElement.resize(ElementType::ElementEnd);
        m_KeyElement[0] = L"Element_Dark";
        m_KeyElement[1] = L"Element_Earth";
        m_KeyElement[2] = L"Element_Fire";
        m_KeyElement[3] = L"Element_Light";
        m_KeyElement[4] = L"Element_Water";
        m_KeyElement[5] = L"Element_Wind";
    }


    {
        m_NpcName.resize(IDX(NPCTYPE::MAX));
        m_NpcName[0] = L"아리엘";
        m_NpcName[1] = L"가챠맨";
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
        _uint iIndex = IDX(QUESTINDEX::KILL_ALPACA);
        m_DialogAccept[iIndex].push_back(L"사냥퀘0");
        m_DialogAccept[iIndex].push_back(L"사냥퀘1");
        m_DialogAccept[iIndex].push_back(L"사냥퀘2");
        m_DialogAccept[iIndex].push_back(L"사냥퀘3");

        m_DialogNoClear[iIndex].push_back(L"깨고오세요");
        m_DialogNoClear[iIndex].push_back(L"깨고와라");

        m_DialogClear[iIndex].push_back(L"깨고왔구나");
        m_DialogClear[iIndex].push_back(L"테스트");
        m_DialogClear[iIndex].push_back(L"끝");

        m_Quest[iIndex].IsClear = false;
        m_Quest[iIndex].Type = QUESTTYPE::HUNT;
        m_Quest[iIndex].CurCount = 0;
        m_Quest[iIndex].MaxCount = 10;
        m_Quest[iIndex].Name = L"kill alpha-car";
        m_Quest[iIndex].Info = L"알파카 학살자";
        m_Quest[iIndex].Clear = L"알파카 컷";
    }

    
    {
        _uint iIndex = IDX(QUESTINDEX::KILL_DELLONS);
        m_DialogAccept[iIndex].push_back(L"계승자님 저희 마을에 있던 알파카가 사라졌어요 \n 도와주실 수 있을까요?");
        m_DialogAccept[iIndex].push_back(L"알파카를 훔쳐간 도둑을 처단해주세요");

        m_DialogNoClear[iIndex].push_back(L"아직인가요?");

        m_DialogClear[iIndex].push_back(L"어라? 알파카는 어디있죠?");

        m_Quest[iIndex].IsClear = false;
        m_Quest[iIndex].Type = QUESTTYPE::BOSS;
        m_Quest[iIndex].CurCount = 0;
        m_Quest[iIndex].MaxCount = 0;
        m_Quest[iIndex].Name = L"도둑 잡기";
        m_Quest[iIndex].Info = L"알파카를 훔쳐간 델론즈를 잡아라";
        m_Quest[iIndex].Clear = L"마을에 돌아가 보고하자";
    }

    

    m_BossDialog.resize(IDX(BOSS::MAX));
    {
        auto& Dialog = m_BossDialog[IDX(BOSS::DELLONS)];
        
        Dialog.push_back(L"크큭 내안의 어둠이 날뛴다 !!");
        Dialog.push_back(L"나는 어둠의 다크 델론즈");
        Dialog.push_back(L"자 덤벼라 애송아");
    }




}

_uint DataMgr::Get_Dialog_Size(QUESTINDEX eIndex, _bool bHaveQuest, _uint iIndex, _bool bIsClear)
{
    if (QUESTINDEX::MAX == eIndex)
        return 0;

    _uint iQuestIndex = IDX(eIndex);
    if (false == bHaveQuest)
        return IDX(m_DialogAccept[iQuestIndex].size());

    else if (false == bIsClear)
        return IDX(m_DialogNoClear[iQuestIndex].size());

    else
        return IDX(m_DialogClear[iQuestIndex].size());
}

const wstring& DataMgr::Get_Dialog(QUESTINDEX eIndex, _bool bHaveQuest, _uint iIndex, _bool bIsClear)
{
    if (QUESTINDEX::MAX == eIndex)
        return strTemp;

    _uint iQuestIndex = IDX(eIndex);
    if (false == bHaveQuest)
        return m_DialogAccept[iQuestIndex][iIndex];

    else if (false == bIsClear)
        return m_DialogNoClear[iQuestIndex][iIndex];

    else
        return m_DialogClear[iQuestIndex][iIndex];
}
