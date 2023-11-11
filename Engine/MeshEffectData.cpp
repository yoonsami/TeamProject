#include "pch.h"
#include "MeshEffectData.h"

#include "Utils.h"

MeshEffectData::MeshEffectData()
	:ResourceBase(ResourceType::MeshEffectData)
{
}

MeshEffectData::~MeshEffectData()
{
}

void MeshEffectData::Set_Desc(DESC tDesc)
{
    m_tDesc = tDesc;
    wstrTag = Utils::ToWString(m_tDesc.pszTag);
}
