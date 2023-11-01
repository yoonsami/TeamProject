#include "pch.h"
#include "MathUtils.h"

_bool MathUtils::PointInSphere(const Point3D& point, const Sphere3D& sphere)
{
    return (point - sphere.position).LengthSquared() <= sphere.radius * sphere.radius;
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const Sphere3D& sphere)
{  
    _float3 dir = point - sphere.position;
    dir.Normalize();

    return sphere.position + dir * sphere.radius;
}

_bool MathUtils::PointInAABB(const Point3D& point, const AABB3D& aabb)
{
    Point3D min = AABB3D::GetMin(aabb);
    Point3D max = AABB3D::GetMax(aabb);

    if (point.x < min.x || point.y < min.y || point.z < min.z)
        return false;

    if (point.x > max.x || point.y > max.y || point.z > max.z)
        return false;

    return true;
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const AABB3D& aabb)
{
    Point3D result = point;
	Point3D minPt = AABB3D::GetMin(aabb);
	Point3D maxPt = AABB3D::GetMax(aabb);

    result.x = max(result.x, minPt.x);
    result.y = max(result.y, minPt.y);
    result.z = max(result.z, minPt.z);

	result.x = min(result.x, maxPt.x);
	result.y = min(result.y, maxPt.y);
	result.z = min(result.z, maxPt.z);

    return result;
}

_bool MathUtils::PointInOBB(const Point3D& point, const OBB3D& obb)
{
    _float3 dir = point - obb.position;

    vector<_float3> axis;
    axis.push_back(obb.orientation.Right());
    axis.push_back(obb.orientation.Up());
    axis.push_back(obb.orientation.Backward());
   
    vector<_float> size;
    
    size.push_back(obb.size.x);
    size.push_back(obb.size.y);
    size.push_back(obb.size.z);

    for (_int i = 0; i < 3; ++i)
    {
        _float dist = dir.Dot(axis[i]);

        if (dist > size[i])
            return false;
        if (dist < -size[i])
            return false;   
    }
    return true;
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const OBB3D& obb)
{
    Point3D result = obb.position;
	_float3 dir = point - obb.position;

	vector<_float3> axis;
	axis.push_back(obb.orientation.Right());
	axis.push_back(obb.orientation.Up());
	axis.push_back(obb.orientation.Backward());

	vector<_float> size;

	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (_int i = 0; i < 3; ++i)
	{
		_float dist = dir.Dot(axis[i]);

        if (dist > size[i])
            dist = size[i];
		if (dist < -size[i])
            dist = -size[i];

        result = result + (axis[i] * dist);
	}

    return result;
}

_bool MathUtils::PointOnPlane(const Point3D& point, const Plane3D& plane)
{
    return CMP(point.Dot(plane.normal),plane.distance);
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const Plane3D& plane)
{
    _float dist = point.Dot(plane.normal) - plane.distance;

    return point - plane.normal * dist;
}

_bool MathUtils::PointInLine(const Point3D& point, const Line3D& line)
{
    Point3D closest = ClosestPoint(point,line);

    return CMP((closest - point).LengthSquared(),0.f);
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const Line3D& line)
{
    _float3 lVec = line.end - line.start;

    _float t = (point - line.start).Dot(lVec) / lVec.Dot(lVec);
    t = fmaxf(t, 0.f);
    t = fmaxf(t, 1.f);

    return line.start + lVec * t;
}

_bool MathUtils::PointOnRay(const Point3D& point, const Ray3D& ray)
{
    if (CMP((point - ray.origin).LengthSquared(),0.f))
        return true;

    _float3 norm = point - ray.origin;
    norm.Normalize();

    _float diff = norm.Dot(ray.direction);
    return CMP(diff,1.f);
}

Point3D MathUtils::ClosestPoint(const Point3D& point, const Ray3D& ray)
{
    _float t = (point - ray.origin).Dot(ray.direction);

    t = fmaxf(t, 0.f);


    return Point3D(ray.origin + ray.direction * t);
}

_bool MathUtils::SphereSphere(const Sphere3D& s1, const Sphere3D& s2)
{
    _float sum = s1.radius + s2.radius;
    _float sqDistance = (s1.position - s2.position).LengthSquared();


    return sqDistance <= sum * sum;
}

_bool MathUtils::SphereAABB(const Sphere3D& s, const AABB3D& aabb)
{
    Point3D closestPoint = ClosestPoint(s.position, aabb);
    _float sqDistance = (s.position - closestPoint).LengthSquared();
    _float sqRadius = s.radius * s.radius;


    return sqDistance < sqRadius;
}

_bool MathUtils::SphereOBB(const Sphere3D& s, const OBB3D& obb)
{
	Point3D closestPoint = ClosestPoint(s.position, obb);
	_float sqDistance = (s.position - closestPoint).LengthSquared();
	_float sqRadius = s.radius * s.radius;


	return sqDistance < sqRadius;
}

_bool MathUtils::AABBAABB(const AABB3D& aabb1, const AABB3D& aabb2)
{
    Point3D aMin = AABB3D::GetMin(aabb1);
    Point3D aMax = AABB3D::GetMax(aabb1); 
	Point3D bMin = AABB3D::GetMin(aabb2);
	Point3D bMax = AABB3D::GetMax(aabb2);


    return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
        (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
        (aMin.z <= bMax.z && aMax.z >= bMin.z);
}

_bool MathUtils::AABBOBB(const AABB3D& aabb, const OBB3D& obb)
{
    _float3 test[15] =
    {
        _float3(1.f,0.f,0.f),       // AABB Axis 1
        _float3(0.f,1.f,0.f),       // AABB Axis 2
        _float3(0.f,0.f,1.f),       // AABB Axis 3
        obb.orientation.Right(),    //OBB Axis 1
        obb.orientation.Up(),       //OBB Axis 2
        obb.orientation.Backward() //OBB Axis 3
    };
    for (_int i = 0; i < 3; ++i)
    {
        test[6 + i * 3 + 0] = test[i].Cross(test[0]);
        test[6 + i * 3 + 1] = test[i].Cross(test[1]);
        test[6 + i * 3 + 2] = test[i].Cross(test[2]);
    }

    for (_int i = 0; i < 15; ++i)
    {
        if (OverlapOnAxis(aabb, obb, test[i]) == false)
            return false;
    }

    return true;
}

_bool MathUtils::AABBPlane(const AABB3D& aabb, const Plane3D& plane)
{

    _float pLen = aabb.size.x * fabsf(plane.normal.x) +
        aabb.size.y * fabsf(plane.normal.y) +
        aabb.size.z * fabsf(plane.normal.z);

    _float dot = plane.normal.Dot(aabb.position);
    _float dist = dot - plane.distance;
    return fabsf(dist) <= pLen;
}

_bool MathUtils::OBBOBB(const OBB3D& obb1, const OBB3D& obb2)
{
	_float3 test[15] =
	{
		obb1.orientation.Right(),         // AABB Axis 1
		obb1.orientation.Up(),            // AABB Axis 2
		obb1.orientation.Backward(),       // AABB Axis 3
		obb2.orientation.Right(),    //OBB Axis 1
		obb2.orientation.Up(),       //OBB Axis 2
		obb2.orientation.Backward() //OBB Axis 3
	};
	for (_int i = 0; i < 3; ++i)
	{
		test[6 + i * 3 + 0] = test[i].Cross(test[0]);
		test[6 + i * 3 + 1] = test[i].Cross(test[1]);
		test[6 + i * 3 + 2] = test[i].Cross(test[2]);
	}

	for (_int i = 0; i < 15; ++i)
	{
		if (OverlapOnAxis(obb1, obb2, test[i]) == false)
			return false;
	}

	return true;
}

_bool MathUtils::PlanePlane(const Plane3D& plane1, const Plane3D& plane2)
{
	_float3 d = plane1.normal.Cross(plane2.normal);
	return !(CMP(d.Dot(d), 0.f));
}

Interval3D MathUtils::GetInterval(const AABB3D& aabb, const _float3& axis)
{
	_float3 i = AABB3D::GetMin(aabb);
	_float3 a = AABB3D::GetMax(aabb);

	_float3 vertex[8] =
	{
		_float3(i.x,a.y,a.z),
		_float3(i.x,a.y,i.z),
		_float3(i.x,i.y,a.z),
		_float3(i.x,i.y,i.z),
		_float3(a.x,a.y,a.z),
		_float3(a.x,a.y,i.z),
		_float3(a.x,i.y,a.z),
		_float3(a.x,i.y,i.z)
	};

	Interval3D result;
    result.min = result.max = axis.Dot(vertex[0]);
    for (_int i = 1; i < 8; ++i)
    {
        _float proj = axis.Dot(vertex[i]);
        result.min = min(result.min, proj);
        result.max = max(result.max, proj);
    }

    return result;
}

Interval3D MathUtils::GetInterval(const OBB3D& obb, const _float3& axis)
{
    _float3 C = obb.position; // Center
    _float3 E = obb.size; // Extents

    _float3 vertex[8];

    vector<_float3> A; // Axis
    A.push_back(obb.orientation.Right());
    A.push_back(obb.orientation.Up());
    A.push_back(obb.orientation.Backward());

    vertex[0] = C + A[0] * E.x + A[1] * E.y + A[2] * E.z;
    vertex[1] = C - A[0] * E.x + A[1] * E.y + A[2] * E.z;
    vertex[2] = C + A[0] * E.x - A[1] * E.y + A[2] * E.z;
    vertex[3] = C + A[0] * E.x + A[1] * E.y - A[2] * E.z;
    vertex[4] = C - A[0] * E.x - A[1] * E.y - A[2] * E.z;
    vertex[5] = C + A[0] * E.x - A[1] * E.y - A[2] * E.z;
    vertex[6] = C - A[0] * E.x + A[1] * E.y - A[2] * E.z;
    vertex[7] = C - A[0] * E.x - A[1] * E.y + A[2] * E.z;

	

	Interval3D result;
	result.min = result.max = axis.Dot(vertex[0]);
	for (_int i = 1; i < 8; ++i)
	{
		_float proj = axis.Dot(vertex[i]);
		result.min = min(result.min, proj);
		result.max = max(result.max, proj);
	}

	return result;
}

_bool MathUtils::OverlapOnAxis(const AABB3D& aabb, const OBB3D& obb, const _float3& axis)
{
    Interval3D a = GetInterval(aabb, axis);
    Interval3D b = GetInterval(obb, axis);

    return (b.min <= a.max) && (a.min <= b.max);
}

_bool MathUtils::OverlapOnAxis(const OBB3D& obb1, const OBB3D& obb2, const _float3& axis)
{
	Interval3D a = GetInterval(obb1, axis);
	Interval3D b = GetInterval(obb2, axis);

	return (b.min <= a.max) && (a.min <= b.max);
}

_bool MathUtils::RayCast(const Sphere3D& sphere, const Ray3D& ray, OUT _float& distance)
{
    _float3 e = sphere.position - ray.origin;
    _float rSq = sphere.radius * sphere.radius;

    _float eSq = e.LengthSquared();

    _float a = e.Dot(ray.direction);

    _float bSq = eSq - (a * a);
    _float f = sqrtf((rSq - bSq));

    if (rSq - bSq < 0.f)
        return false;

    if (eSq < rSq)
    {
        distance = a + f;
        return true;
    }

    distance = a - f;
    return true;
}

_bool MathUtils::RayCast(const AABB3D& aabb, const Ray3D& ray, OUT _float& distance)
{
    _float3 min = AABB3D::GetMin(aabb);
    _float3 max = AABB3D::GetMax(aabb);

    _float t1 = (min.x - ray.origin.x) / ray.direction.x;
    _float t2 = (max.x - ray.origin.x) / ray.direction.x;

    _float t3 = (min.y - ray.origin.y) / ray.direction.y;
    _float t4 = (max.y - ray.origin.y) / ray.direction.y;

	_float t5 = (min.z - ray.origin.z) / ray.direction.z;
	_float t6 = (max.z - ray.origin.z) / ray.direction.z;

    // Largest Min Value
    _float tMin = fmaxf(fmaxf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));
    // Smallest Max Value
    _float tMax = fminf(fminf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));

    if (tMax < 0.f)
        return false;

    if (tMin > tMax)
        return false;

    if (tMin < 0.f)
    {
        distance = tMax;
        return true;
    }

    distance = tMin;
    return true;
}

_bool MathUtils::RayCast(const Plane3D& plane, const Ray3D& ray, OUT _float& distance)
{
    _float nd = ray.direction.Dot(plane.normal);
    _float pn = ray.origin.Dot(plane.normal);

    if (nd >= 0.f)
        return false;

    _float t = (plane.distance - pn) / nd;

    if (t >= 0.f)
    {
        distance = t;
        return true;
    }
    return false;
}

_bool MathUtils::PointInTriangle(const Point3D& p, const Triangle3D& t)
{
    _float3 a = t.a - p;
    _float3 b = t.b - p;
    _float3 c = t.c - p;

    _float3 normPBC = b.Cross(c);
    _float3 normPCA = c.Cross(a);
    _float3 normPAB = a.Cross(b);

    if (normPBC.Dot(normPCA) < 0.f)
        return false;

    else if (normPBC.Dot(normPAB) < 0.f)
        return false;

    return true;
    
}

Plane3D MathUtils::FromTriangle(const Triangle3D& t)
{
    Plane3D result;

    result.normal = (t.b - t.a).Cross(t.c - t.a);
    result.normal.Normalize();

    result.distance = result.normal.Dot(t.a);
    return result;
}

_float3 MathUtils::Barycentric(const Point3D& p, const Triangle3D& t)
{

    return _float3(0.f);
}

_bool MathUtils::RayCast(const Triangle3D& tri, const Ray3D& ray, OUT _float& distance)
{
    Plane3D plane = FromTriangle(tri);

    _float t = 0.f;
    if (RayCast(plane, ray, OUT t) == false)
        return false;

    Point3D result = ray.origin + ray.direction * t;

    _float3 barycentric = Barycentric(result, tri);

    if (barycentric.x >= 0.f && barycentric.x <= 1.f &&
        barycentric.y >= 0.f && barycentric.y <= 1.f &&
        barycentric.z >= 0.f && barycentric.z <= 1.f)
    {
        distance = t;
        return true;
    }

    return false;
}

_float3 MathUtils::ProjectVecOnVec(_float3 a, _float3 b)
{
    b.Normalize();
    _float dist = a.Dot(b);


    return b * dist;
}

