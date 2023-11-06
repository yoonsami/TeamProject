#pragma once
#include "MonoBehaviour.h"
#include "ModelRenderer.h"
class MapObjectScript :
    public MonoBehaviour
{
public:
    MapObjectScript(shared_ptr<ModelRenderer> _renderer, ModelRenderer::INSTANCE_PASSTYPE _PassType = ModelRenderer::PASS_MAPOBJECT, _float _fUVWeight = 1.f);

public:
    _float Get_UVWeight() { return m_fUVWeight; }

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

//private:
//    weak_ptr<ModelRenderer> m_pRenderer;
//    ModelRenderer::INSTANCE_PASSTYPE _PassType = ModelRenderer::PASS_MAPOBJECT;
    _float m_fUVWeight = 1.f;
};

