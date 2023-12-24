#include "pch.h"
#include "MapObjectLoopRotate.h"

MapObjectLoopRotate::MapObjectLoopRotate(_float4 _RotateData)
 : m_vecRotateData(_RotateData)
{
}

void MapObjectLoopRotate::Tick()
{
    if (!m_pOwner.expired())
    {
        m_pOwner.lock()->Get_Transform()->Turn(_float3{ m_vecRotateData }, XMConvertToRadians(m_vecRotateData.w));
    }
}