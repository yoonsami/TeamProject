#pragma once
#include "MonoBehaviour.h"

class WeaponScript : public MonoBehaviour
{
public:
	typedef struct tagWeaponDesc
	{
        wstring strBoneName = L"";
        _float4x4 matPivot = XMMatrixIdentity();
        weak_ptr<GameObject> pWeaponOwner;
	}WEAPONDESC;

public:
    WeaponScript(WEAPONDESC weapondesc);
    ~WeaponScript();

public:
    virtual HRESULT Init() override;
    virtual void Late_Tick() override;

private:
    wstring m_strBoneName = L"";
    _float4x4 m_matPivot = XMMatrixIdentity();
    weak_ptr<GameObject> m_pWeaponOwner;
};

