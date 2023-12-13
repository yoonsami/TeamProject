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
	DUMMY_ARIA,
	DUMMY_BANE,
	DUMMY_ENT,
	DUMMY_GOBLIN,
	DUMMY_ONI,
	DUMMY_PANDA,
	DUMMY_NAGA,
	DUMMY_SCOTT,
	DUMMY_VAMPIRE,
	DUMMY_WEREWOLF,
	DUMMY_BRANSEL,
	DUMMY_DARKARMY_AXE,
	DUMMY_DARKARMY_CROSSBOW,
	DUMMY_DARKARMY_SWORD,
	DUMMY_DEA,
	DUMMY_FERUS,
	DUMMY_GEHENNA,
	DUMMY_HAERI,
	DUMMY_JUNO,
	DUMMY_KAGURA,
	DUMMY_KARIN,
	DUMMY_KARMA,
	DUMMY_LINA,
	DUMMY_LUCIA,
	DUMMY_MELYN,
	DUMMY_MERCURE,
	DUMMY_WATT,
	DUMMY_OSCAR,
	DUMMY_PALLANUS,
	DUMMY_PASCAL,
	DUMMY_PHYSYS1,
	DUMMY_PHYSYS2,
	DUMMY_PLANE,
	DUMMY_PLATON,
	DUMMY_RACHEL1,
	DUMMY_RACHEL2,
	DUMMY_RADIN,
	DUMMY_RADISAND,
	DUMMY_SILVESTA,
	DUMMY_SUCCUBUS,
	DUMMY_VANESSA,
	MAX
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

enum class SCENE_TYPE
{
	GRANSEED,
	FIELD,
	MIR,
	KRIS,
	SPIKE,
};

enum ObjectType
{
	OBJ_NONE,
	OBJ_TEAM,
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
	POTION,
	HIDE_KID,
	SPIKE,
	DELLONS,
	SHANE,
	PORTAL,
	MAX
};

enum class QUESTINDEX
{

	TRY_GACHA,
	KILL_ALPACA,
	KILL_DELLONS,
	KILL_SPIKE,
	KILL_MIR,
	STORY_CLEAR,
	HIDE_AND_SEEK,
	MAX
};

enum class QUESTTYPE
{
	ACT,
	COLLECT,
	HUNT,
	BOSS,
	MAX
};

enum class CUR_QUEST 
{ 
	NONE,
	ACCEPT, 
	PROGRESS, 
	CLEAR, 
	OTHER, 
	MAX 
};