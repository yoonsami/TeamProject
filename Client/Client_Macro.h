#pragma once

#define DEBUGTOOL                   GET_SINGLE(DebugTool)
#define NORMAL_ATTACK               L"Normal_Attack"
#define KNOCKBACK_ATTACK            L"KnockBack_Attack"
#define KNOCKDOWN_ATTACK            L"KnockDown_Attack"
#define AIRBORNE_ATTACK             L"Airborne_Attack"

#define NORMAL_SKILL                L"Normal_Skill"
#define KNOCKBACK_SKILL             L"KnockBack_Skill"
#define KNOCKDOWN_SKILL             L"KnockDown_Skill"
#define AIRBORNE_SKILL              L"Airborne_Skill"

#define IDX(VALUE)                  static_cast<unsigned int>(VALUE)

#define GET_DATA(VALUE)             GET_SINGLE(DataMgr).Get_Data(VALUE)
#define GET_ELEMENT(VALUE)          GET_SINGLE(DataMgr).Get_Element(VALUE)
#define GET_ELEMENT_LINE(VALUE)     GET_SINGLE(DataMgr).Get_Element_Line(VALUE)

