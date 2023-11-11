#pragma once
#include "BaseCollider.h"

class Model;
template<typename>
class Geometry;

class MeshCollider :
    public BaseCollider
{
public:
    MeshCollider(wstring modelTag ,_float4x4 MatWorld = _float4x4::Identity);
    MeshCollider(Mesh& _Mesh);
    ~MeshCollider();

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual _bool Intersects(Ray& ray, OUT _float& distance) override;
	virtual _bool Intersects(shared_ptr<BaseCollider>& others) override;
    virtual InstanceID Get_InstanceID() override;
	virtual TransformDesc Get_TransformDesc() override;
	virtual _float3 Get_CenterPos() override { return _float3(FLT_MAX); };
	wstring Get_ModelTag() { return m_strModelTag; }
	virtual _bool Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& dist) override;
	virtual _float3 Get_Extend() override { return _float3(FLT_MAX); }
	virtual _float3 Get_ClosestPoint(shared_ptr<BaseCollider> pCollider) override;

private:
	shared_ptr<Model> m_pModel;
	wstring m_strModelTag;
};

