#pragma once

enum PLANE_TYPE :_uchar
{
	PLANE_FRONT,
	PLANE_BACK,
	PLANE_UP,
	PLANE_DOWN,
	PLANE_LEFT,
	PLANE_RIGHT,

	PLANE_END
};

class Frustum
{
public:
	void Final_Tick(const _float4x4& view, const _float4x4& proj);


	_bool Contain_Sphere(const _float3& pos, _float radius);
	const _float4& Get_Plane(PLANE_TYPE type) { return _planes[type]; }
private:
	array<_float4, PLANE_END> _planes;
};

