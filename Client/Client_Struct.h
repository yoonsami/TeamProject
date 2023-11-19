#pragma once

typedef struct tagSkillTotalInfo
{
	_bool			bIsWork			= false;
	_bool			bIsEnd			= false;
	SkillInfo		eSKillInfo		= SkillInfo::NONE;
	_uint			iSkillCombo		= 0;
	_uint			iCurCombo		= 0;
	COOLTIMEINFO	CoolInfo		= {};
}TOTALSKILLINFO;

typedef struct tagHeroData
{
	ElementType Element = ElementType::ElementEnd;

	_uint		Star				= 1;

	_uint		Skill0Combo			= 0;
	_uint		Skill1Combo			= 0;
	_uint		Skill2Combo			= 0;
	_uint		Skill3Combo			= 0;
	_uint		Skill4Combo			= 0;
	_uint		Skill5Combo			= 0;

	SkillInfo	Skill1Info			= SkillInfo::NONE;
	SkillInfo	Skill2Info			= SkillInfo::NONE;
	SkillInfo	Skill3Info			= SkillInfo::NONE;
	SkillInfo	Skill4Info			= SkillInfo::NONE;
	SkillInfo	Skill5Info			= SkillInfo::NONE;

	float		AttackDamage		= 1.f;
	float		MaxHp				= 10.f;
	float		CurHp				= 10.f;
	float		PreHp				= 10.f;
	float		Deffence			= 1.f;
	float		HpRegen				= 1.f;
	float		CriPercent			= 0.f;
	float		CriDamageMul		= 1.f;

	float		Skill1Cool			= 0.f;
	float		Skill2Cool			= 0.f;
	float		Skill3Cool			= 0.f;
	float		Skill4Cool			= 0.f;
	float		Skill5Cool			= 0.f;

	float		Skill1DamageMul		= 1.f;
	float		Skill2DamageMul		= 1.f;
	float		Skill3DamageMul		= 1.f;
	float		Skill4DamageMul		= 1.f;
	float		Skill5DamageMul		= 1.f;

	wstring		ModelTag;
	wstring		WeaponTag;
	wstring		KeyAttack;
	wstring		KeyEvade;
	wstring		KeySkill1;
	wstring		KeySkill2;
	wstring		KeySkill3;
	wstring		KeySkill4;
	wstring		KeySkill5;
	wstring		KeyChangeCard;
	wstring		KeyDeckMini;
	wstring		KeyDeckSelect;

}HERODATA;

