#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"
#include "InstallationSkill_Script.h"
#include "FloorSkill_Script.h"


class Client_FSM abstract :
    public FSM
{

protected:
	void AttackCollider_On(const wstring& skillname, _float fAttackDamage);
	void AttackCollider_Off();

	void Set_ColliderOption(ElementType eType, const wstring& strHitEffectTag);
	void Create_ForwardMovingSkillCollider(CollisionGroup eGroup, const wstring& strColliderName, const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage);
	void Create_InstallationSkillCollider(CollisionGroup eGroup, const wstring& strColliderName, const _float4& vPos, _float fSkillRange, INSTALLATIONSKILLDESC desc);
	void Create_FloorSkillCollider(CollisionGroup eGroup, const wstring& strColliderName, const _float4& vPos, _float3 vSkillScale, FLOORSKILLDESC desc);

};

