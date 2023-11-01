#pragma once

struct VTXDATA
{
	_float3 position = { 0, 0, 0 };
};

struct VTXTEXDATA
{
	_float3 vPosition = {0,0,0};
	_float2 vTexCoord = { 0,0 };

};

struct VTXBillboard
{
	_float3 vPosition = { 0,0,0 };
	_float2 vTexCoord = { 0,0 };
	_float2 vScale = { 0,0 };
};

struct VTXCOLORDATA
{
	_float3 vPosition{ 0,0,0 };
	_float4 vColor{ 0,0,0,0 };
};


struct VTXTEXNORDATA
{
	_float3 vPosition = { 0, 0, 0 };
	_float2 vTexCoord = { 0, 0 };
	_float3 vNormal = { 0, 0, 0 };
};

struct VTXTEXNORTANDATA
{
	_float3 vPosition = { 0, 0, 0 };
	_float2 vTexCoord = { 0, 0 };
	_float3 vNormal = { 0, 0, 0 };
	_float3 vTangent = { 0,0,0 };
};

struct VTXTEXNORTANBlendDATA
{
	_float3 vPosition = { 0, 0, 0 };
	_float2 vTexCoord = { 0, 0 };
	_float3 vNormal = { 0, 0, 0 };
	_float3 vTangent = { 0,0,0 };
	_float4 vBlendIndices = { 0,0,0,0 };
	_float4 vBlendWeights = { 0,0,0,0 };
	_int iFiltered = 0;
};

using ModelVertexType = VTXTEXNORTANBlendDATA;