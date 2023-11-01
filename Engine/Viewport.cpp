#include "pch.h"
#include "Viewport.h"

Viewport::Viewport()
{
    Set(1920, 1080);
}

Viewport::Viewport(_float width, _float height, _float x, _float y, _float minDepth, _float maxDepth)
{
    Set(width, height, x, y, minDepth, maxDepth);
}

void Viewport::RSSetViewport()
{
    CONTEXT->RSSetViewports(1, &m_Viewport);
}

void Viewport::Set(_float width, _float height, _float x, _float y, _float minDepth, _float maxDepth)
{
    m_Viewport.TopLeftX = x;
    m_Viewport.TopLeftY = y;

    m_Viewport.Width = width;
    m_Viewport.Height = height;
    m_Viewport.MinDepth = minDepth;
    m_Viewport.MaxDepth = maxDepth;
}

_float3 Viewport::Projection(const _float3& vLocalPos, const _float4x4& matWorld, const _float4x4& matView, const _float4x4& matProj)
{
    _float4x4 WVP = matWorld * matView * matProj;

    _float3 vProjectionPos = _float3::Transform(vLocalPos, WVP);

    vProjectionPos.x = (vProjectionPos.x + 1.f) * (m_Viewport.Width / 2) + m_Viewport.TopLeftX;
    vProjectionPos.y = (-vProjectionPos.y + 1.f) * (m_Viewport.Height / 2) + m_Viewport.TopLeftY;
    vProjectionPos.z = vProjectionPos.z * (m_Viewport.MaxDepth - m_Viewport.MinDepth) + m_Viewport.MinDepth;

    return vProjectionPos;
}

_float3 Viewport::UnProjection(const _float3& vProjectionPos, const _float4x4& matWorld, const _float4x4& matView, const _float4x4& matProj)
{
    _float3 vLocalPos;
    vLocalPos.x = 2.f * (vProjectionPos.x - m_Viewport.TopLeftX) / m_Viewport.Width - 1.f;
    vLocalPos.y = -2.f * (vProjectionPos.y - m_Viewport.TopLeftY) / m_Viewport.Height + 1.f;
    vLocalPos.z =( (vProjectionPos.z - m_Viewport.MinDepth) / (m_Viewport.MaxDepth - m_Viewport.MinDepth));

	_float4x4 WVP = matWorld * matView * matProj;
    _float4x4 WVPInv = WVP.Invert();

    vLocalPos = _float3::Transform(vLocalPos, WVPInv);

    return vLocalPos;
}
