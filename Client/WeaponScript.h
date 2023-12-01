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
    void Set_ModelChanged() { m_bModelChanged = true; }
    virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<WeaponScript>(WEAPONDESC{m_strBoneName,m_matPivot,m_pWeaponOwner.lock()}); }

private:
    wstring m_strBoneName = L"";
    _float4x4 m_matPivot = XMMatrixIdentity();
    weak_ptr<GameObject> m_pWeaponOwner;
    _bool m_bModelChanged = false;
};

