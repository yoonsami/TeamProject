#pragma once
#include "ResourceBase.h"

class ParticleData final : public ResourceBase
{
public:
	typedef struct tagParticleData
	{
		const char* pszTag;
	}DESC;

public:
	ParticleData();
	~ParticleData();

public:
	void	Set_Desc(DESC tDesc);

	wstring Get_ParticleDataTag() { return m_wstrTag; }
	DESC	Get_Desc() { return m_tDesc; }

private:
	wstring	m_wstrTag;
	DESC	m_tDesc;
};

