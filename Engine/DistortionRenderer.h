#pragma once
#include "Component.h"
class DistortionRenderer :
    public Component
{
public:
    DistortionRenderer(shared_ptr<Shader> shader);
    ~DistortionRenderer();

public:
    virtual HRESULT Init() override;
    
    void Render();

public:
	void Set_Material(shared_ptr<Material> material);
	void Set_Mesh(shared_ptr<Mesh> mesh) { m_pMesh = mesh; }
	void Set_Pass(_uint pass) { m_iPass = pass; }

	void Set_Model(shared_ptr<Model> model);
	shared_ptr<Model> Get_Model() { return m_pModel; }

	_uint Get_Pass() { return m_iPass; }
	shared_ptr<Mesh> Get_Mesh() { return m_pMesh; }
	shared_ptr<Material> Get_Material() { return  m_pMaterial; }
	shared_ptr<Shader> Get_Shader() { return m_pShader; }

private:
	shared_ptr<Shader> m_pShader;
	shared_ptr<Mesh> m_pMesh;
	shared_ptr<Model> m_pModel;
	shared_ptr<Material> m_pMaterial;
	_uint m_iPass = 0;

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

