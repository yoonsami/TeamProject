#pragma once
#include "Component.h"
class ModelAnimator;

class MotionTrailRenderer :
    public Component
{
	enum PASS_INFO
	{
		PS_ANIM = 0,
		PS_NONANIM = 1,
	};

public:
	MotionTrailRenderer(shared_ptr<Shader> shader, shared_ptr<ModelAnimator> animtor);
	MotionTrailRenderer(shared_ptr<Shader> shader, const TweenDesc& desc, shared_ptr<Model> model);
	MotionTrailRenderer(shared_ptr<Shader> shader, shared_ptr<Model> model);
	
	~MotionTrailRenderer();

public:
    virtual void Tick() override;

	void Render();
	//void Render_Instancing(shared_ptr<class InstancingBuffer>& buffer, shared_ptr<InstanceTweenDesc> desc, shared_ptr<InstanceRenderParamDesc> renderParamDesc);

	shared_ptr<Model> Get_Model() { return m_pModel; }
	shared_ptr<Shader> Get_Shader() { return m_pShader; }

	//InstanceID Get_InstanceID();
	TweenDesc& Get_TweenDesc() { return m_TweenDesc; }
	//TweenDesc& Get_preTweenDesc() { return m_preTweenDesc; }

private:
	TweenDesc   m_TweenDesc;
	TweenDesc   m_preTweenDesc;
	_uint m_iPass = 0;
	shared_ptr<Shader> m_pShader;
	shared_ptr<Model> m_pModel;

	_float m_fTrailScale = { 1.f };

public:
	auto& Get_RenderParamDesc() { return m_RenderParams; }
	void SetInt(_uchar index, _int value) { m_RenderParams.SetInt(index, value); }
	void SetFloat(_uchar index, _float value) { m_RenderParams.SetFloat(index, value); }
	void SetVec2(_uchar index, _float2 value) { m_RenderParams.SetVec2(index, value); }
	void SetVec4(_uchar index, _float4 value) { m_RenderParams.SetVec4(index, value); }
	void SetMatrix(_uchar index, _float4x4& value) { m_RenderParams.SetMatrix(index, value); }
	void Set_Scale(_float fScale);
private:
	RenderParams m_RenderParams{};

};

