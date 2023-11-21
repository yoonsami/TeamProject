#pragma once
#include "Component.h"
#include "GroupEffectData.h"

class GroupEffect final : public Component,public enable_shared_from_this<GroupEffect>
{
public:
	GroupEffect();
	virtual ~GroupEffect();

public:
	virtual void        Init(void* pArg);
	virtual void        Tick() override;
	virtual void        Final_Tick() override;

	void				Save(const wstring& path);
	void				DeleteMember(const wstring& wstrTag);

	void				FreeLoopMember();
	void				Kill_All();

	/* Setter */
	void				Set_Tag(wstring wstrTag) { m_wstrTag = wstrTag; }
	void				Set_MemberEffectData(vector <GroupEffectData::MemberEffect_Desc> vData) { m_vMemberEffectData = vData; }
	void				Set_Member_StartTime(_int iIndex, _float fStartTime) { m_vMemberEffectData[iIndex].fCreateTime = fStartTime; }
	void				Set_Member_PivotPos(_int iIndex, _float3 vPos) { m_vMemberEffectData[iIndex].vPivot_Pos = vPos; }
	void				Set_Member_PivotScale(_int iIndex, _float3 vScale) { m_vMemberEffectData[iIndex].vPivot_Scale = vScale; }
	void				Set_Member_PivotRotation(_int iIndex, _float3 vRotation) { m_vMemberEffectData[iIndex].vPivot_Rotation = vRotation; }
	void				Set_InitWorldMatrix(const _float4x4& mInitMat) { m_mInitWorldMatrix = mInitMat; }

	/* Getter */
	vector <GroupEffectData::MemberEffect_Desc> Get_MemberEffectData() { return m_vMemberEffectData; }

private:
	void				Create_MeshEffect(_int iIndex);
	void				Create_Particle(_int iIndex);

private:
	/* Data */
	wstring                             m_wstrTag;
	vector <GroupEffectData::MemberEffect_Desc> m_vMemberEffectData;

	_float                              m_fCurrAge = { 0.f };
	_float                              m_fTimeAcc_CreatCoolTime = { 0.f };

	_bool								m_bIsFirstTick = { false };
	_float4x4							m_mInitWorldMatrix;

	vector<shared_ptr<GameObject>>      m_vMemberEffects;
};
