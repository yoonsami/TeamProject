#pragma once
#include "BaseCollider.h"

class MeshCollider;

class AABBBoxCollider :
    public BaseCollider
{
public:
    AABBBoxCollider(const _float3& boundingSize);
    virtual ~AABBBoxCollider();

    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual TransformDesc Get_TransformDesc() override;
    virtual _float3 Get_Extend() override { return m_BoundingBox.Extents; }
	virtual _float3 Get_ClosestPoint(shared_ptr<BaseCollider> pCollider) override;
    virtual _float3 Get_CenterPos() override { return m_BoundingBox.Center; };
    virtual _bool Intersects(Ray& ray, OUT _float& distance) override;
    virtual _bool Intersects(shared_ptr<BaseCollider>& others) override;
    virtual _bool Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& dist) override;
    BoundingBox& Get_BoundingBox() { return m_BoundingBox; }

private:
    BoundingBox m_BoundingBox;

};

