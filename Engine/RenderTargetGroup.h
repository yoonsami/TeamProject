#pragma once
#include "Texture.h"

enum class RENDER_TARGET_GROUP_TYPE : _uchar
{
	SWAP_CHAIN,
	SHADOW,
	MOTIONBLUR,
	G_BUFFER,// POSITION,NORMAL, DEPTH , DIFFUSE, SPECULAR, EMISSIVE, BLUR
	BLURSMALLER0,
	BLURSMALLER1,
	BLURBIGGER0,
	BLURBIGGER1,
	SSAO,
	SSAODOWNSCALE0,
	SSAODOWNSCALE1,
	SSAODOWNSCALE2,
	SSAOUPSCALE0,
	SSAOUPSCALE1,
	SSAOUPSCALE2,
	LIGHTING,// AMBIENT, DIFFUSE, SPECULAR, EMISSIVE
	FINAL, // FINAL

	OUTLINE,
	OUTLINEFINAL,

	MOTIONBLURFINAL,
	BLOOMMAP,
	BLOOMDOWNSCALE0,
	BLOOMDOWNSCALE1,
	BLOOMDOWNSCALE2,
	BLOOMUPSCALE0,
	BLOOMUPSCALE1,
	BLOOMUPSCALE2,
	BLOOMFINAL,

	DOFMAP,
	DOFDOWNSCALE0,
	DOFDOWNSCALE1,
	DOFDOWNSCALE2,
	DOFUPSCALE0,
	DOFUPSCALE1,
	DOFUPSCALE2,
	DOFFINAL,

	DISTORTION, // DISTORTIONEFFECT
	DISTORTIONFINAL,
	FXAA,
	LENSFLARE,

	LENSFLAREFINAL,
	ABERRATION,
	FOG,

	TONEMAPPING,

	SUBSCENEFINAL,

	END,
};

enum
{
	RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_MOTIONBLUR_GROUP_MEMBER_COUNT =1,

	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 7,
	RENDER_TARGET_BLUR_GROUP_MEMBER_COUNT = 1,
	
	RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT = 4,
	RENDER_TARGET_FINAL_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_OUTLINE_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_OUTLINEFINAL_GROUP_MEMBER_COUNT =1,

	RENDER_TARGET_SSAO_GROUP_MEMBER_COUNT =1,


	RENDER_TARGET_MOTIONBLURFINAL_GROUP_MEMBER_COUNT =1,

	RENDER_TARGET_BLOOMMAP_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_BLOOMSCALE_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_BLOOMFINAL_GROUP_MEMBER_COUNT =1,

	RENDER_TARGET_DOFSCALE_GROUP_MEMBER_COUNT = 1,
	RENDER_TARGET_DOFFINAL_GROUP_MEMBER_COUNT = 1,

	RENDER_TARGET_DISTORTION_GROUP_MEMBER_COUNT =2,
	RENDER_TARGET_DISTORTIONFINAL_GROUP_MEMBER_COUNT =1,

	RENDER_TARGET_FXAA_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_LENSFLARE_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_LENSFLARESCALE_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_LENSFLAREFINAL_GROUP_MEMBER_COUNT =1,

	RENDER_TARGET_ABERAATION_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_FOG_GROUP_MEMBER_COUNT =1,
	RENDER_TARGET_TONEMAPPING_GROUP_MEMBER_COUNT =1,

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

