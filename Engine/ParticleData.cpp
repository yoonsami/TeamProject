#include "pch.h"
#include "ParticleData.h"

#include "Utils.h"

ParticleData::ParticleData()
	:ResourceBase(ResourceType::ParticleData)
{
}

ParticleData::~ParticleData()
{
}

void ParticleData::Set_Desc(DESC tDesc)
{
	m_tDesc = tDesc;
	m_wstrTag = Utils::ToWString(m_tDesc.pszTag);
}
