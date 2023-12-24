#pragma once
#include "MonoBehaviour.h"
#include "pch.h"

class MapObjectLoopRotate :
    public MonoBehaviour
{
public:
    MapObjectLoopRotate(_float4 _RotateData);
    virtual ~MapObjectLoopRotate() = default;

public:
    virtual void Tick() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return nullptr; }

public:
    void Set_RotateData(_float4 _RotateData) { m_vecRotateData = _RotateData; }

private:
    // RotateAxis, RotateSpeed
    _float4 m_vecRotateData = { 0.f, 0.f, 1.f, 1.f };
};