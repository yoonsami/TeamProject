#pragma once
enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };


const unsigned int	g_iWinSizeX = 1920;
const unsigned int	g_iWinSizeY = 1080;

extern HWND		g_hWnd;


typedef struct tagCoolTimeInfo
{
	_float fCoolTime;
	_float fAccTime = 0.f;
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

	Script,

	END = MAX_COLLIDER_GROUP
};

#define DEBUGTOOL GET_SINGLE(DebugTool)
#define NORMAL_SKILL    L"Normal_Skill"
#define KNOCKDOWN_SKILL L"KnockDown_Skill"
#define AIRBORNE_SKILL  L"Airborne_Skill"

