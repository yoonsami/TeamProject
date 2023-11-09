#include "pch.h"
#include "SoundMgr.h"

#include "Utils.h"

SoundMgr::~SoundMgr()
{
	m_pSystem->release();
	m_pSystem->close();
}

HRESULT SoundMgr::Initialize()
{
	if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
	{
		MSG_BOX("Failed System_Create");
		return E_FAIL;
	}

	if (m_pSystem->init(32, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
	{
		MSG_BOX("Failed Sound init");
		return E_FAIL;
	}

	/*if (m_pSystem->createChannelGroup("ListenerGroup", &m_pListenerGroup) != FMOD_OK)
	{
		MSG_BOX("Failed createChannelGroup");
		return E_FAIL;
	}

	m_pListenerGroup->setMode(FMOD_3D);

	FMOD_VECTOR vPos = { 0,0,0 };
	FMOD_VECTOR vVel = { 0,0,0 };
	FMOD_VECTOR vForward = { 0,0,0 };
	FMOD_VECTOR vUp = { 0,0,0 };
	FMOD_RESULT result = m_pListenerGroup->set3DAttributes(&vPos, &vVel);
	if (result != FMOD_OK)
	{
		MSG_BOX("Failed set3DAttributes");
		return E_FAIL;
	}

	if (m_pSystem->getMasterChannelGroup(&m_pSoundGroup) != FMOD_OK)
	{
		MSG_BOX("Failed getMasterChannelGroup");
		return E_FAIL;
	}

	if (m_pSoundGroup->addGroup(m_pListenerGroup) != FMOD_OK)
	{
		MSG_BOX("Failed addGroup");
		return E_FAIL;
	}*/
	return S_OK;
}

void SoundMgr::Tick()
{
	if (CUR_SCENE && CUR_SCENE->Get_MainCamera())
	{
		const _float4x4& matWorld = CUR_SCENE->Get_MainCamera()->Get_Transform()->Get_WorldMatrix();
		FMOD_VECTOR vPos = { matWorld.Translation().x,matWorld.Translation().y,matWorld.Translation().z };
		FMOD_VECTOR vVel = { 0,0,0 };
		FMOD_VECTOR vLook = { matWorld.Backward().x, matWorld.Backward().y, matWorld.Backward().z };
		FMOD_VECTOR vUp = { matWorld.Up().x,matWorld.Up().y,matWorld.Up().z };

		m_pSystem->set3DListenerAttributes(0, &vPos, &vVel, &vLook, &vUp);

		for (_int i = 0; i < (_uint)SOUND_EFFECT; ++i)
		{
			bool bIsPlaying = false;
			m_pChannelArr[i]->isPlaying(&bIsPlaying);
			if (bIsPlaying)
			{
				FMOD_VECTOR vNewPos = { vPos.x + vLook.x,vPos.y + vLook.y ,vPos.z + vLook.z };
				m_pChannelArr[i]->set3DAttributes(&vPos, &vVel);
			}
		}
		m_pSystem->update();
	}

}

void SoundMgr::Play_Sound(const wstring& strSoundTag, CHANNELID eID, float fVolume, _uint iLevelIndex, const _float3& vPos, const _float2& vMinMaxDist)
{
	if (eID < 0 || eID >= MAXCHANNEL)
		return;

	shared_ptr<CustomSound> sound = RESOURCES.Get<CustomSound>(strSoundTag);
	if (!sound)
		return;

	bool bIsPlaying = false;

	if (eID < SOUND_EFFECT)
	{
		m_pChannelArr[eID]->isPlaying(&bIsPlaying);

		if (!bIsPlaying)
			m_pSystem->playSound(sound->Get_Sound(), 0, false, &(m_pChannelArr[eID]));
		
		m_pChannelArr[eID]->setVolume(fVolume);
	}
	else
	{
		for (_uint i = (_uint)SOUND_EFFECT; i < (_uint)MAXCHANNEL; ++i)
		{
			m_pChannelArr[i]->isPlaying(&bIsPlaying);
			if (bIsPlaying)
				continue;

			FMOD_VECTOR position = { vPos.x,vPos.y,vPos.z };
			FMOD_VECTOR vel = { 0.f,0.f,0.f };

			m_pSystem->playSound(sound->Get_Sound(), 0, false, &m_pChannelArr[i]);
			m_pChannelArr[i]->setVolume(fVolume);
			m_pChannelArr[i]->set3DMinMaxDistance(vMinMaxDist.x, vMinMaxDist.y);
			m_pChannelArr[i]->set3DAttributes(&position, &vel);
			break;
		}
	}

	m_pSystem->update();

}

void SoundMgr::Pause_Sound(CHANNELID eID)
{
	m_pChannelArr[eID]->setPaused(true);
}

void SoundMgr::Resume_Sound(CHANNELID eID)
{
	m_pChannelArr[eID]->setPaused(false);
}

void SoundMgr::StopSound(CHANNELID eID)
{
	if (eID < 0 || eID >= MAXCHANNEL)
		return;

	FMOD_RESULT hr = m_pChannelArr[eID]->stop();
	if (hr == FMOD_OK)
		int a = 0;
}

void SoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		StopSound(CHANNELID(i));
}

FMOD::Channel* SoundMgr::Get_Channel()
{
	for (_uint i = (_uint)SOUND_EFFECT; i < (_uint)MAXCHANNEL; ++i)
	{
		bool bIsPlaying = false;
		m_pChannelArr[i]->isPlaying(&bIsPlaying);
		if(!bIsPlaying)
			return m_pChannelArr[i];
	}

	return nullptr;
}
