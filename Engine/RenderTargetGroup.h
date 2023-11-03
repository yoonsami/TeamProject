#pragma once
#include "Texture.h"

enum class RENDER_TARGET_GROUP_TYPE : _uchar
{
	SWAP_CHAIN,
	SHADOW,
	G_BUFFER,// POSITION,NORMAL, AMBIENT , DIFFUSE, SPECULAR, EMISSIVE, BLUR
	BLURSMALLER0,
	BLURSMALLER1,
	BLURBIGGER0,
	BLURBIGGER1,
	LIGHTING,// AMBIENT, DIFFUSE, SPECULAR, EMISSIVE

	DISTORTION, // DISTORTIONEFFECT
	FINAL, // FINAL
	END,
};

enum
{
	RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT =1,

	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 7,
	RENDER_TARGET_BLUR_GROUP_MEMBER_COUNT = 1,
	
	RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT = 4,
	RENDER_TARGET_FINAL_GROUP_MEMEBER_COUNT =1,
	RENDER_TARGET_DISTORTION_GROUP_MEMEBER_COUNT =2,
	RENDER_TARGET_GROUP_COUNT = static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::END),
};

struct RenderTarget
{
	shared_ptr<Texture> target;
	_float4 clearColor;
};

class RenderTargetGroup
{
public:
	void Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture>DepthStencilTexture);

	void OMSetRenderTarget(_uint count, _uint offset);
	void OMSetRenderTargets();

	void ClearRenderTargetView(_uint index);
	void ClearRenderTargetView();

	void UnBindSRV();

	shared_ptr<Texture> GetRenderTargetTexture(_uint index) { return m_RenderTargets[index].target; }
	shared_ptr<Texture> GetDepthStencilTexture() { return m_pDepthStencilTexture; }
	
private:
	RENDER_TARGET_GROUP_TYPE		m_eGroupType{};
	vector<RenderTarget>			m_RenderTargets;
	_uint							m_iRenderTargetCount{};
	shared_ptr<Texture>				m_pDepthStencilTexture;
	ComPtr< ID3D11RenderTargetView> m_pRTV;
};

