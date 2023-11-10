#pragma once
#include "ResourceBase.h"

class GroupEffectData final : public ResourceBase
{
public: 
	typedef struct tagGroupEffectData
	{
		const char* pszTag;
	}DESC;

public:
	GroupEffectData();
	~GroupEffectData();

public:
	void	Set_Desc(DESC tDesc);

	wstring Get_GroupEffectDataTag() { return m_wstrTag; }
	DESC	Get_Desc() { return m_tDesc; }

private:
	wstring	m_wstrTag;
	DESC	m_tDesc;
};

