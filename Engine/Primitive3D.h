#pragma once

using Point3D = _float3;

struct Line3D
{
	Point3D start = Point3D(0.f);
	Point3D end = Point3D(0.f);

	_float Length() { return _float3::Distance(start, end); }
	_float LengthSq() { return _float3::DistanceSquared(start, end); }
};

struct Ray3D
{
	Point3D origin = Point3D(0.f);
	_float3 direction = _float3(0.f);

	void NormalizeDirection() { direction.Normalize(); }

};

struct Sphere3D
{
	Point3D position;
	_float radius;
};

struct AABB3D
{
	Point3D position;
	_float3 size = _float3(1, 1, 1);

	static _float3 GetMin(const AABB3D& aabb)
	{
		_float3 p1 = aabb.position + aabb.size;
		_float3 p2 = aabb.position - aabb.size;
		return _float3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}

	static _float3 GetMax(const AABB3D& aabb)
	{
		_float3 p1 = aabb.position + aabb.size;
		_float3 p2 = aabb.position - aabb.size;
		return _float3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}

	static AABB3D FromMinMax(const _float3& min, const _float3 max)
	{
		return AABB3D{ (min + max) / 2.f , (max - min) / 2.f };
	}
};

struct OBB3D
{
	OBB3D(const BoundingOrientedBox& box)
	{
		position = box.Center;
		size = box.Extents;
		orientation = _float4x4::CreateFromQuaternion(box.Orientation);
	}
	Point3D position;
	_float3 size;
	_float4x4 orientation;

};

struct Plane3D
{
	_float3 normal;
	_float distance;


};

struct Triangle3D
{
	union 
	{
		struct 
		{
			Point3D a;
			Point3D b;
			Point3D c;
		};
		Point3D points[3];
		float value[9];

	};
	
};

struct Interval3D
{
	float min;
	float max;
};
