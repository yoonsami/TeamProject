#pragma once

enum class HERO 
{ 
	PLAYER,
	ACE3,
	KYLE,
	YEOPO,
	DELLONS,
	SPIKE,
	YEONHEE,
	SHANE,
	MAX 
};

enum class DUMMY_HERO
{

};

enum class BOSS
{
	MIR,
	AXE,
	SPIKE,
	DELLONS,
	MAX
};

enum LEVEL
{ 
	LEVEL_STATIC,
	LEVEL_LOADING,
	LEVEL_LOGO,
	LEVEL_GAMEPLAY,
	LEVEL_END
};

enum ObjectType
{
	OBJ_PLAYER,
	OBJ_MONSTER,
	OBJ_NPC,
	OBJ_MAPOBJECT,
	OBJ_END
};

enum CollisionGroup
{
	Player_Body,
	Player_Attack,
	Player_Skill,

	Monster_Body,
	Monster_Attack,
	Monster_Skill,

	MAPObject,

	Script,

	END = MAX_COLLIDER_GROUP
};

enum SkillType
{
	SKILL1,		// 1번 스킬
	SKILL2,		// 2번 스킬
	SKILL3,		// 3번 스킬
	SKILL4,		// 4번 스킬
	SKILL5,		// 시너지 스킬
	EVADE,		// 회피
	DEFAULT,	// 기본 공격
	SkillEnd
};

enum ElementType
{
	DARK,
	EARTH,
	FIRE,
	LIGHT,
	WATER,
	WIND,
	ElementEnd
};

enum SkillInfo
{
	CHARGING,
	HOLDING,
	MOVING,
	NONE
};

enum class LevelInfo
{
	STATIC,
	LOGO,
};

enum class NPCTYPE
{
	QUEST,
	GACHA,
	MAX
};

enum class QUESTDIALOG
{
	TRY_GACHA,
	KILL_MONSTER,
	KILL_MIR,
	MAX
};