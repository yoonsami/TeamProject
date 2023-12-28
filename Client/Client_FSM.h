#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"
#include "InstallationSkill_Script.h"
#include "FloorSkill_Script.h"

class Client_FSM abstract :
    public FSM
{

public:
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;

	virtual void Create_ForwardMovingSkillCollider(CollisionGroup eGroup, const wstring& strColliderName, const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage, const wstring& hitEffectTag = L"");

protected:
	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage);
	virtual void AttackCollider_Off();

	void Set_On_Ground();
	void Set_ColliderOption(ElementType eType, const wstring& strHitEffectTag);
	void Create_InstallationSkillCollider(CollisionGroup eGroup, const wstring& strColliderName, const _float4& vPos, _float fSkillRange, INSTALLATIONSKILLDESC desc);
	void Create_FloorSkillCollider_Sphere(CollisionGroup eGroup, const wstring& strColliderName, const _float4& vPos, _float fSkillRange, FLOORSKILLDESC desc);
	void Create_FloorSkillCollider(CollisionGroup eGroup, const wstring& strColliderName, const _float4& vPos, _float3 vSkillScale, FLOORSKILLDESC desc);
	void Create_Foot_Dust();

protected:
	ElementType m_eElementType = ElementEnd;
	_float m_fStTimer = 0.f;
};

