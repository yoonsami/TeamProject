#pragma once

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

const unsigned int	g_iWinSizeX = 1920;
const unsigned int	g_iWinSizeY = 1080;

extern HWND		g_hWnd;

typedef struct tagCoolTimeInfo
{
	_float fCoolTime;				// Max CoolTime
	_float fAccTime		= 0.f;		// Check Time
	_bool  bIsWork		= false;
}COOLTIMEINFO;

enum ObjectType
{
	OBJ_PLAYER,
	OBJ_MONSTER,
	OBJ_NPC,
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

#define DEBUGTOOL GET_SINGLE(DebugTool)
#define NORMAL_ATTACK    L"Normal_Attack"
#define KNOCKBACK_ATTACK    L"KnockBack_Attack"
#define KNOCKDOWN_ATTACK    L"KnockDown_Attack"
#define AIRBORNE_ATTACK    L"Airborne_Attack"

#define NORMAL_SKILL    L"Normal_Skill"
#define KNOCKBACK_SKILL L"KnockBack_Skill"
#define KNOCKDOWN_SKILL L"KnockDown_Skill"
#define AIRBORNE_SKILL  L"Airborne_Skill"

#define IDX(VALUE) static_cast<_uint>(VALUE) // cast _uint