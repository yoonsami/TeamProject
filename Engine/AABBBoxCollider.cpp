#include "pch.h"
#include "AABBBoxCollider.h"
#include "SphereCollider.h"
#include "Camera.h"
#include "OBBBoxCollider.h"
#include "MeshCollider.h"

AABBBoxCollider::AABBBoxCollider(const _float3& boundingSize)
	:BaseCollider(ColliderType::AABB)
{
	m_vExtent = boundingSize;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->CreateCube();

	m_Meshes.push_back(mesh);

}

AABBBoxCollider::~AABBBoxCollider()
{
}

HRESULT AABBBoxCollider::Init()
{
	m_vOffset = Get_Transform()->Get_State(Transform_State::UP).xyz() * m_BoundingBox.Extents.y * 0.5f;

	return S_OK;
}

void AABBBoxCollider::Tick()
{
	Add_ColliderGroup();
	m_BoundingBox.Center = _float3::Transform(m_vOffset,Get_Transform()->Get_WorldMatrix());
	_float3 scale = Get_Transform()->Get_Scale();
	m_BoundingBox.Extents.x = m_vExtent.x * scale.x;
	m_BoundingBox.Extents.y = m_vExtent.y * scale.y;
	m_BoundingBox.Extents.z = m_vExtent.z * scale.z;
}

TransformDesc AABBBoxCollider::Get_TransformDesc()
{
	auto world = _float4x4::CreateScale(m_BoundingBox.Extents) * _float4x4::CreateTranslation(m_BoundingBox.Center);

	return TransformDesc{ world };
}

_float3 AABBBoxCollider::Get_ClosestPoint(shared_ptr<BaseCollider> pCollider)
{
	
	return _float3();
}

_bool AABBBoxCollider::Intersects(Ray& ray, OUT _float& distance)
{
	return m_BoundingBox.Intersects(ray.position, ray.direction, OUT distance);
}

_bool AABBBoxCollider::Intersects(shared_ptr<BaseCollider>& others)
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

_bool AABBBoxCollider::Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& outDist)
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
