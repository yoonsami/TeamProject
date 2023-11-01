#pragma once
#include "BaseCollider.h"
class SphereCollider : public BaseCollider
{
public:
    SphereCollider(_float radius);
    virtual ~SphereCollider();

public:
    BoundingSphere& Get_BoundingSphere() { return m_BoundingSphere; }
public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual _bool Intersects(Ray& ray, OUT _float& distance) override;
	virtual _bool Intersects(shared_ptr<BaseCollider>& others) override;
	virtual TransformDesc Get_TransformDesc() override;
    virtual _float3 Get_Extend() override { return _float3(m_BoundingSphere.Radius); }
	virtual _bool Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& dist) override;
    virtual _float3 Get_ClosestPoint(shared_ptr<BaseCollider> pCollider) override;
    virtual _float3 Get_CenterPos() override { return m_BoundingSphere.Center; }
private:
    BoundingSphere m_BoundingSphere;
    _float m_fRadius = 1.f;

};

