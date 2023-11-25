#pragma once

#define DEBUGTOOL                   GET_SINGLE(DebugTool)


#define NONE_HIT					L""
#define NORMAL_ATTACK               L"Normal_Attack"
#define NORMAL_ATTACK_COUNTER       L"Normal_Attack_Counter"
#define KNOCKBACK_ATTACK            L"KnockBack_Attack"
#define KNOCKBACK_ATTACK_COUNTER    L"KnockBack_Attack_Counter"
#define KNOCKDOWN_ATTACK            L"KnockDown_Attack"
#define KNOCKDOWN_ATTACK_COUNTER    L"KnockDown_Attack_Counter"
#define AIRBORNE_ATTACK             L"Airborne_Attack"
#define AIRBORNE_ATTACK_COUNTER     L"Airborne_Attack_Counter"

#define NORMAL_SKILL                L"Normal_Skill"
#define NORMAL_SKILL_COUNTER        L"Normal_Skill_Counter"
#define KNOCKBACK_SKILL             L"KnockBack_Skill"
#define KNOCKBACK_SKILL_COUNTER     L"KnockBack_Skill_Counter"
#define KNOCKDOWN_SKILL             L"KnockDown_Skill"
#define KNOCKDOWN_SKILL_COUNTER     L"KnockDown_Skill_Counter"
#define AIRBORNE_SKILL              L"Airborne_Skill"
#define AIRBORNE_SKILL_COUNTER      L"Airborne_Skill_Counter"

#define IDX(VALUE)                  static_cast<unsigned int>(VALUE)

#define GET_DATA(VALUE)             GET_SINGLE(DataMgr).Get_Data(VALUE)
#define GET_ELEMENT(VALUE)          GET_SINGLE(DataMgr).Get_Element(VALUE)
#define GET_ELEMENT_LINE(VALUE)     GET_SINGLE(DataMgr).Get_Element_Line(VALUE)

#define GET_NPC_NAME(VALUE)         GET_SINGLE(DataMgr).Get_Npc_Name(VALUE)
#define GET_QUEST_COUNT             GET_SINGLE(DataMgr).Get_Quest_Count()
#define GET_DIALOG_COUNT(VALUE)     GET_SINGLE(DataMgr).Get_Quest_Dialog_Count(VALUE)
#define GET_DIALOG(VALUE, INDEX)    GET_SINGLE(DataMgr).Get_Quest_Dialog(VALUE, INDEX)

#define GET_CUR_QUEST_NAME(VALUE)   GET_SINGLE(DataMgr).Get_Cur_Quest_Name(VALUE)
#define GET_CUR_QUEST_INFO(VALUE)   GET_SINGLE(DataMgr).Get_Cur_Quest_Info(VALUE)
#define GET_CUR_QUEST_CLEAR(VALUE)  GET_SINGLE(DataMgr).Get_Cur_Quest_Clear(VALUE)
#define GET_MO_CLEAR(VALUE)         GET_SINGLE(DataMgr).Get_No_Clear(VALUE)