#include "pch.h"
#include "Frustum.h"

#include "Camera.h"

void Frustum::Final_Tick(const _float4x4& view, const _float4x4& proj)
{
	_float4x4 matViewInv = view.Invert();
	_float4x4 matProjectionInv = proj.Invert();
	_float4x4 matInv = matProjectionInv * matViewInv;

	vector<_float3> worldPos =
	{
		::XMVector3TransformCoord(_float3(-1.f,1.f,0.f),matInv),
		::XMVector3TransformCoord(_float3(1.f,1.f,0.f),matInv),
		::XMVector3TransformCoord(_float3(1.f,-1.f,0.f),matInv),
		::XMVector3TransformCoord(_float3(-1.f,-1.f,0.f),matInv),

		::XMVector3TransformCoord(_float3(-1.f,1.f,1.f),matInv),
		::XMVector3TransformCoord(_float3(1.f,1.f,1.f),matInv),
		::XMVector3TransformCoord(_float3(1.f,-1.f,1.f),matInv),
		::XMVector3TransformCoord(_float3(-1.f,-1.f,1.f),matInv)
	};

	_planes[PLANE_FRONT] = XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]);
	_planes[PLANE_BACK] = XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]);
	_planes[PLANE_UP] = XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]);
	_planes[PLANE_DOWN] = XMPlaneFromPoints(worldPos[3], worldPos[2], worldPos[7]);
	_planes[PLANE_LEFT] = XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]);
	_planes[PLANE_RIGHT] = XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]);
}

_bool Frustum::Contain_Sphere(const _float3& pos, _float radius)
{
	for (auto& plane : _planes)
	{
		_float3 normal = _float3(plane.x, plane.y, plane.z);

		if (normal.Dot(pos) + plane.w > radius)
			return false;
	}

	return true;
}
