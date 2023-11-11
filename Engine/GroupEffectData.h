#pragma once
#include "ResourceBase.h"

class GroupEffectData final : public ResourceBase
{
public:
	enum EFFECT_TYPE { TYPE_MESHEFFECT, TYPE_PARTICLE, TYPE_END };

	typedef struct tagMemberEffectProperty
	{
		wstring		wstrEffectTag;
		EFFECT_TYPE	eType;
		_float		fCreateTime;

		_bool		bIsActive = false;

		_float3		vPivot_Pos;
		_float3		vPivot_Scale;
		_float3		vPivot_Rotation;

	}MemberEffect_Desc;

public:
	GroupEffectData();
	~GroupEffectData();

public:
	void	Set_Tag(wstring wstrTag) { m_wstrTag = wstrTag; }
	void	Set_MemberEffectData(vector<MemberEffect_Desc> vData) { m_vMemberEffectData = vData; }

	void	Add_MemberEffect(MemberEffect_Desc tDesc);

	wstring							Get_GroupEffectDataTag() { return m_wstrTag; }
	vector<MemberEffect_Desc>		Get_MemberEffectData() { return m_vMemberEffectData; }

	virtual void					Load(const wstring& path) override;

private:
	wstring							m_wstrTag;
	vector<MemberEffect_Desc>		m_vMemberEffectData;
};

