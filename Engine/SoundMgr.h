#pragma once
#include "CustomSound.h"

enum CHANNELID
{
	SOUND_BGM,
	SOUND_GLOBAL,
	SOUND_UI,
	SOUND_EFFECT,
	SOUND_NORMAL,
	MAXCHANNEL = 32
};

class SoundMgr
{
	DECLARE_SINGLETON(SoundMgr)

public:
	~SoundMgr();
public:
	HRESULT Initialize();
	void Tick();
	void Play_Sound(const wstring& strSoundTag, CHANNELID eID, float fVolume, _uint iLevelIndex, const _float3& vPos = _float3(0.f), const _float2& vMinMaxDist = _float2(0.f));
	void Pause_Sound(CHANNELID eID);
	void Resume_Sound(CHANNELID eID);
	void StopSound(CHANNELID eID);
	void StopAll();
	FMOD::Channel* Get_Channel();

	FMOD::System* Get_System() { return m_pSystem; }

private:

	array<FMOD::Channel*, MAXCHANNEL> m_pChannelArr{};

	FMOD::System* m_pSystem = nullptr;
	FMOD::ChannelGroup* m_pSoundGroup = nullptr;
	FMOD::ChannelGroup* m_pListenerGroup = nullptr;
	

};
