#pragma once
#include "MonoBehaviour.h"

class MainCameraScript : public MonoBehaviour
{
public:
    MainCameraScript(shared_ptr<GameObject> pPlayer);
    ~MainCameraScript();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual void Late_Tick() override;

public:
    _bool Is_Fixed() { return m_fFixedTime > 0.f; }
    _bool Is_Shaking() { return m_fShakeTime > 0.f; }
    void Fix_Camera(_float fTime, _float3 vDir, _float vDist);
    void ShakeCamera(_float fTime, _float fPower);
    void Set_FixedLookTarget(const _float3& vPos);
    void Set_FixedDir(const _float3& vDir) { m_vFixedDir = vDir; }
    void Set_FixedDist(_float fDist) { m_fFixedDist = fDist; }
    const _float3& Get_FixedDir() { return m_vFixedDir; }
    _float Get_FixedDist() { return m_fFixedDist; }
    const _float3& Get_Offset() { return m_vOffset; }
    void Set_PosDirectly(const _float3& vCenterpos, const _float3& vDir);
    const _float3& Get_FixedPos() { return m_vFixedPos; }
    void Set_State(_bool flag) { m_bOn = flag; if (!m_bOn) m_fFixedTime = 0.f; }
    void Set_Offset(const _float3& offset) { m_vOffset = offset; }
    _float Get_ShakingPower() { if (m_fShakeTime > 0) return m_fShakePower; else return 0.f; }
    _float& Get_MaxDist() { return m_fMaxDistance; }
private:
    void Cal_OffsetDir();
    void Restrict_Offset();
    void Update_Transform();

    void Find_Target();

private:
    _bool m_bOn = true;
    _float m_fFollowSpeed = 1.f;
    _float m_fMaxDistance = 5.f;
    _float3 m_vOffset = { 1.f,0.f,0.f };
    _float m_fRotationSpeed = 1.f;

    weak_ptr<GameObject> m_pPlayer;
    weak_ptr<GameObject> m_pTarget;


    _float m_fMaxHeightRadian = XM_PI / 3.f;
    _float3 m_vFixedPos = _float3(0.f);
    _float3 m_vFixedDir = _float3(0.f);
    _float m_fShakeTime = 0.f;
    _float m_fFixedTime = 0.f;
    _float m_fFixedDist = 0.f;
    _float m_fShakePower = 0.f;
};