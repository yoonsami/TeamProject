#pragma once
class Viewport
{
public:
	Viewport();
	Viewport(_float width, _float height, _float x = 0, _float y = 0, _float minDepth = 0, _float maxDepth = 1);
	~Viewport() = default;


	void RSSetViewport();
	void Set(_float width, _float height, _float x = 0, _float y = 0, _float minDepth = 0, _float maxDepth = 1);

	_float Get_Width() { return m_Viewport.Width; }
	_float Get_Height() { return m_Viewport.Height; }

	_float3 Projection(const _float3& vLocalPos, const _float4x4& matWorld, const _float4x4& matView, const _float4x4& matProj);
	_float3 UnProjection(const _float3& vPos, const _float4x4& matWorld, const _float4x4& matView, const _float4x4& matProj);


private:
	D3D11_VIEWPORT m_Viewport;
};

