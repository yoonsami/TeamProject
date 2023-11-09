#include "pch.h"
#include "CustomSound.h"

#include "Utils.h"

CustomSound::CustomSound()
	: ResourceBase(ResourceType::Sound)
{
}

CustomSound::~CustomSound()
{
	m_pSound->release();
}

void CustomSound::Load(const wstring& path)
{
	FMOD_MODE mode{};

	if (path.find(L"Non3D") != wstring::npos)
	{
		mode = FMOD_LOOP_OFF;
	}
	else
		mode = FMOD_LOOP_OFF | FMOD_3D;

	if (SOUND.Get_System()->createSound(Utils::ToString(path).c_str(), mode, 0, &m_pSound) != FMOD_OK)
	{
		MSG_BOX("Failed to LoadSound");
		assert(false);
	}
}
