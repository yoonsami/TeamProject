#pragma once
#include "Component.h"
#include "GroupEffectData.h"

class GroupEffect final : public Component
{
public:
	GroupEffect();
	virtual ~GroupEffect();

public:
	virtual void			Init(void* pArg);
	virtual void			Tick() override;
	virtual void			Final_Tick() override;      
	void					Save(const wstring& path);
	
	/* Setter */
	void					Set_Loop(_bool bIsLoop) { m_bIsLoop = bIsLoop; }
	void					Set_Tag(wstring wstrTag) { m_wstrTag = wstrTag; }
	void					Set_MemberEffectData(vector <GroupEffectData::MemberEffect_Desc> vData) { m_vMemberEffectData = vData; }	

private:
	void					Create_MeshEffect(_int iIndex);
	void					Create_Particle(_int iIndex);

private:	
	/* Data */
	wstring										m_wstrTag;
	vector <GroupEffectData::MemberEffect_Desc> m_vMemberEffectData;

	_float										m_fCurrAge = { 0.f };
	_bool										m_bIsLoop = { true };
	vector<shared_ptr<GameObject>>				m_vMemberEffects;
};

