#pragma once
#include "Geometry.h"
#include "VertexData.h"
class GeometryHelper
{
public:

	static void CreateQuad(shared_ptr<Geometry<VTXCOLORDATA>> geometry, _float4 color);

	static void CreateQuad(shared_ptr<Geometry<VTXTEXDATA>> geometry);
	static void CreateCube(shared_ptr<Geometry<VTXTEXDATA>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VTXTEXDATA>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VTXTEXDATA>> geometry, _uint iSizeX, _uint iSizeZ);

	static void CreateQuad(shared_ptr<Geometry<VTXTEXNORDATA>> geometry);
	static void CreateCube(shared_ptr<Geometry<VTXTEXNORDATA>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VTXTEXNORDATA>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VTXTEXNORDATA>> geometry, _uint iSizeX, _uint iSizeZ);

	// 점페어(좌하단 우하단페어) 벡터를 통해 벽메시를 만들어줌
	static void Create3DRect(shared_ptr<Geometry<VTXTEXNORTANDATA>> _geometry, vector<pair<_float3, _float3>> _LDURPairVector);
	// 점페어(좌상단 우하단) 벡터를 통해 사각형모음메시(바닥)를 만듦 
	static void CreateGround(shared_ptr<Geometry<VTXTEXNORTANDATA>> _geometry, vector<pair<_float3,_float3>> _LURDPointsVector);
	static void CreateQuad(shared_ptr<Geometry<VTXTEXNORTANDATA>> geometry);
	static void CreateCube(shared_ptr<Geometry<VTXTEXNORTANDATA>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VTXTEXNORTANDATA>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VTXTEXNORTANDATA>> geometry, _uint iSizeX, _uint iSizeZ);

	static void CreatePoint(shared_ptr<Geometry<VTXTEXNORTANDATA>> geometry);
};

