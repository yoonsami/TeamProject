#include "pch.h"
#include "GroupEffectData.h"

#include "Utils.h"

GroupEffectData::GroupEffectData()
	:ResourceBase(ResourceType::GroupEffectData)
{
}

GroupEffectData::~GroupEffectData()
{
}

void GroupEffectData::Set_Desc(DESC tDesc)
{
	m_tDesc = tDesc;
	m_wstrTag = Utils::ToWString(m_tDesc.pszTag);
}
