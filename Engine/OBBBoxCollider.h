#pragma once
#include "BaseCollider.h"
class OBBBoxCollider :
    public BaseCollider
{
public:
	OBBBoxCollider(const _float3& boundingSize);
	virtual ~OBBBoxCollider();

	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual _bool Intersects(Ray& ray, OUT _float& distance) override;
	virtual _bool Intersects(shared_ptr<BaseCollider>& others) override;
	virtual TransformDesc Get_TransformDesc() override;
	virtual _float3 Get_CenterPos() override { return m_BoundingBox.Center; };
	virtual _float3 Get_Extend() override { return m_BoundingBox.Extents ; }
	BoundingOrientedBox& Get_BoundingBox() { return m_BoundingBox; }
	virtual _bool Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& dist) override;
	virtual _float3 Get_ClosestPoint(shared_ptr<BaseCollider> pCollider) override;
	void Set_BoundingSize(const _float3& size) { m_vExtent = size; }
	const _float3& Get_BoundingSize() { return m_vExtent; }
private:
	BoundingOrientedBox m_BoundingBox;


};

