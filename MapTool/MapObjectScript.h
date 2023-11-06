#pragma once
#include "MonoBehaviour.h"
#include "ModelRenderer.h"
#include "BaseCollider.h"
class MapObjectScript :
    public MonoBehaviour
{
public:
    typedef struct MapObjectDesc
    {
        string Name = "";
        // 계산해서 넣기
        _float3 CullPos = _float3{ 0.f, 0.f, 0.f };
        _float CullRadius = { 0.f };
        _bool bShadow = false;
        _bool bBlur = false;
    // Component
        // Transform
        _bool bTransform = true;
        _float4x4 WorldMatrix = XMMatrixIdentity();
        // Collider
        _bool bCollider = false;
        //ColliderType ColliderType = ColliderType::OBB;
        _int ColliderType = static_cast<_int>(ColliderType::OBB);
            // OBB Collider
        _float3 ColliderOffset = _float3{ 0.f, 0.f, 0.f };
        _float3 ColliderSize = _float3{ 0.f, 0.f, 0.f };
            // Mesh Collider
        string ModelName = "";
    }MAPOBJDESC;

public:
    MapObjectScript(shared_ptr<ModelRenderer> _renderer, ModelRenderer::INSTANCE_PASSTYPE _PassType = ModelRenderer::PASS_MAPOBJECT, _float _fUVWeight = 1.f);

public:
    _float Get_UVWeight() { return m_fUVWeight; }
    MAPOBJDESC& Get_DESC() { return ; }

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

//private:
    MAPOBJDESC m_MapObjDesc;
    _float m_fUVWeight = 1.f;
};

