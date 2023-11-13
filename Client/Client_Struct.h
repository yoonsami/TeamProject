#pragma once

typedef struct tagCoolTimeInfo
{
	float	fCoolTime	= 0.f;		// Max CoolTime
	float	fAccTime	= 0.f;		// Check Time
	bool	bIsWork		= false;
	bool	bIsEnd		= false;
}COOLTIMEINFO;

typedef struct tagHeroData
{
	ElementType Element = ElementType::ElementEnd;

	float		AttackDamage	= 1.f;
	float		MaxHp			= 10.f;
	float		CurHp			= 10.f;
	float		PreHp			= 10.f;
	float		Deffence		= 1.f;
	float		HpRegen			= 1.f;
	float		CriPercent		= 0.f;
	float		CriDamageMul	= 1.f;

	float		Skill1Cool		= 0.f;
	float		Skill2Cool		= 0.f;
	float		Skill3Cool		= 0.f;
	float		Skill4Cool		= 0.f;
	float		Skill5Cool		= 0.f;

	float		Skill1DamageMul = 1.f;
	float		Skill2DamageMul = 1.f;
	float		Skill3DamageMul = 1.f;
	float		Skill4DamageMul = 1.f;
	float		Skill5DamageMul = 1.f;

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

