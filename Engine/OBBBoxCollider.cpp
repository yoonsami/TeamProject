#include "pch.h"
#include "OBBBoxCollider.h"
#include "SphereCollider.h"
#include "AABBBoxCollider.h"
#include "Camera.h"
#include "MeshCollider.h"
#include "Primitive3D.h"
#include "MathUtils.h"


OBBBoxCollider::OBBBoxCollider(const _float3& boundingSize)
    : BaseCollider(ColliderType::OBB)
{
	m_vExtent = boundingSize;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->CreateCube();

	m_Meshes.push_back(mesh);
}

OBBBoxCollider::~OBBBoxCollider()
{
}

HRESULT OBBBoxCollider::Init()
{
	/*auto physics = Get_Owner()->Get_Physics();
	if (physics)	
	{
		PxBoxGeometry tmp(m_BoundingBox.Extents.x, m_BoundingBox.Extents.y, m_BoundingBox.Extents.z);
		physics->Create_RigidBody(&tmp, m_bStatic);
	}*/
	m_vOffset = Get_Transform()->Get_State(Transform_State::UP).xyz() * m_vExtent.y;

	return S_OK;
}

void OBBBoxCollider::Tick()
{
	Add_ColliderGroup();
	_float4x4 matWorld = Get_Owner()->Get_Transform()->Get_WorldMatrix();
	
	_float3 trans, scale;
	Quaternion q;
	
	matWorld.Decompose(scale, q, trans);

	m_BoundingBox.Center = _float3::Transform(m_vOffset,matWorld);
	m_BoundingBox.Orientation = q;
	m_BoundingBox.Extents.x = m_vExtent.x * scale.x;
	m_BoundingBox.Extents.y = m_vExtent.y * scale.y;
	m_BoundingBox.Extents.z = m_vExtent.z * scale.z;

}

_bool OBBBoxCollider::Intersects(Ray& ray, OUT _float& distance)
{
    return m_BoundingBox.Intersects(ray.position,ray.direction,OUT distance);
}

_bool OBBBoxCollider::Intersects(shared_ptr<BaseCollider>& others)
{
	ColliderType type = others->Get_ColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return m_BoundingBox.Intersects(dynamic_pointer_cast<SphereCollider>(others)->Get_BoundingSphere());

		break;
	case ColliderType::AABB:
		return m_BoundingBox.Intersects(dynamic_pointer_cast<AABBBoxCollider>(others)->Get_BoundingBox());
		break;
	case ColliderType::OBB:
		return m_BoundingBox.Intersects(dynamic_pointer_cast<OBBBoxCollider>(others)->Get_BoundingBox());
		break;
	default:
		break;
	}
	return false;
}

TransformDesc OBBBoxCollider::Get_TransformDesc()
{
	return TransformDesc{ _float4x4::CreateScale(m_BoundingBox.Extents) * _float4x4::CreateFromQuaternion(m_BoundingBox.Orientation) * _float4x4::CreateTranslation(m_BoundingBox.Center) };
}

_bool OBBBoxCollider::Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& outDist)
{
	_float3 vRayDir = -Get_Transform()->Get_State(Transform_State::UP).xyz();
	vRayDir.Normalize();
	_float3 vRayOrigin = m_BoundingBox.Center - vRayDir * m_BoundingBox.Extents.y;

	_float dist = FLT_MAX;

	Ray ray(vRayOrigin, vRayDir);
	_bool collided = others->Intersects(ray, dist);
	if (!collided)
		return false;

	collidePos = vRayOrigin + vRayDir * dist;
	outDist = dist;

	return true;
}

_float3 OBBBoxCollider::Get_ClosestPoint(shared_ptr<BaseCollider> pCollider)
{
	return _float3();
}
