#pragma once
#include "ResourceBase.h"
class CustomSound :
    public ResourceBase
{
public:
    CustomSound();
    ~CustomSound();
public:
    FMOD::Sound* Get_Sound() { return m_pSound; }
    virtual void Load(const wstring& path) override;

private:
    FMOD::Sound* m_pSound = nullptr;
};

