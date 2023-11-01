#pragma once
#include "Component.h"



class Model;
class Shader;
class Material;

class ModelRenderer :
    public Component
{
public:
	enum PASS_INFO
	{
		PS_NONANIM =0,
		PS_NONANIMINSTANCE =1,

		PS_NONANIM_CULLNONE = 4,
		PS_NONANIM_CULLNONE_INSTANCE = 5,

		PS_NONANIM_SHADOW =6,
		PS_NONANIM_SHADOW_INSTANCE =7,

	};
public:
	ModelRenderer(shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	shared_ptr<Model> Get_Model() { return m_pModel; }
	shared_ptr<Shader> Get_Shader() { return m_pShader; }
	void Set_Shader(shared_ptr<Shader> shader) { m_pShader = shader; }
	void Set_Model(shared_ptr<Model> model);
	void Set_Pass(_uint pass) { m_iPass = pass; }

	void Render_Instancing(shared_ptr<class InstancingBuffer>& buffer, shared_ptr<InstanceRenderParamDesc> renderParamDesc);
	void Render();
	void Render_Skybox();
	void Render_Shadow();
	void Render_Shadow_Instancing(shared_ptr<InstancingBuffer>& buffer, shared_ptr<InstanceRenderParamDesc> renderParamDesc);
	void Set_Outline() { m_bHasOutline = true; }
	InstanceID Get_InstanceID();
	void Set_CullMode(_bool cullNone) { m_bCullNone = cullNone; }
	_bool Is_CullNone() { return m_bCullNone; }
	_float2& Get_UVSliding() { return m_vUvSilding; }

private:
	_bool				m_bHasOutline = false;
	shared_ptr<Shader>	m_pShader;
	_int				m_iPass = 0;
	shared_ptr<Model>	m_pModel;
	_bool				m_bCullNone = false;
	_float2				m_vUvSilding = { 0.f,0.f };

public:
	auto& Get_RenderParamDesc() { return m_RenderParams; }
	void SetInt(_uchar index, _int value) { m_RenderParams.SetInt(index, value); }
	void SetFloat(_uchar index, _float value) { m_RenderParams.SetFloat(index, value); }
	void SetVec2(_uchar index, _float2 value) { m_RenderParams.SetVec2(index, value); }
	void SetVec4(_uchar index, _float4 value) { m_RenderParams.SetVec4(index, value); }
	void SetMatrix(_uchar index, _float4x4& value) { m_RenderParams.SetMatrix(index, value); }

private:
	RenderParams m_RenderParams{};
};

