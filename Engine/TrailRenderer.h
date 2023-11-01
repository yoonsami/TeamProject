#pragma once
#include "Component.h"
class TrailRenderer :
    public Component
{
public:
    TrailRenderer(shared_ptr<Shader> shader, _int iMaxTrailCount, _float fTrailSize);
    ~TrailRenderer();

public:
    virtual void Final_Tick() override;
    void Render();

    void Reset();
    void Set_RunState(_bool flag) { m_bRunning = flag; Reset(); }
    shared_ptr<Shader> Get_Shader() { return m_pShader; }
    void Set_Material(shared_ptr<Material> material);
    shared_ptr<Material> Get_Material() { return m_pMaterial; }
    void Set_TrailDir(const _float3& dir) { m_vVertexDir = dir; }
    void Set_CenterPos(const _float3& vCenterPos) { m_vCenterPos = vCenterPos; }

private:
    _float3 m_vCenterPos = _float3(0.f);
    shared_ptr<Shader> m_pShader = nullptr;
    _float3 m_vVertexDir = _float3(0.f);
	list<VTXTEXNORTANDATA> m_Vertices;
	vector<_uint> m_Indices;
    _float m_fTrailSize = 1.f;
	shared_ptr<VertexBuffer> m_pVB;
	shared_ptr<IndexBuffer> m_pIB;

    shared_ptr<Material> m_pMaterial;

    _float m_fCreateInterval = 0.05f;
    _float m_fAccTime = 0.f;

    _int m_iTrailCount = 0;
    _int m_iMaxTrailCount = 20;

    _int m_iPass = 0;

    _bool m_bRunning = false;
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

