#pragma once
#include "MonoBehaviour.h"
#include "Utils.h"
class WeedGroup :
    public Component
{
public:
    WeedGroup();
    virtual ~WeedGroup();

public:
    virtual HRESULT Init() override;

    void Push_Weed(shared_ptr<GameObject> _pWeed) { m_Weeds.push_back(_pWeed); }

public:
    virtual void Tick() override;
    virtual vector<shared_ptr<GameObject>>& Get_Weeds() { return m_Weeds; }
    const _float4& Get_RenderParams() { return _float4(m_Wind.x, m_Wind.y, m_Wind.z, m_fWindWeight); }

private:
    vector<shared_ptr<GameObject>> m_Weeds;
    // 바람방향
    _float4 m_Wind = { -1.f, 0.f, 0.f, 0.f };
    // 바람파워
    //_float m_fWindPower = { 0.05f };
    // 바람의 현재상태 (0~1)
    _float m_fWindWeight = { Utils::Random_In_Range(0.f, 1.f) };
    _float m_fWindWeightPM = { 1.f };
    // 바람의 속도
    _float m_fWindSpeed = { Utils::Random_In_Range(0.9f, 1.1f) };
    // 풀의 모델번호
    _int m_iWeedIndex = 0;
};




