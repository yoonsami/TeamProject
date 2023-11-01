#pragma once
#include "Component.h"
struct ParticleInfo
{
	_float3	worldPos =_float3(0.f);
	_float	curTime = 0.f;
	_float3 worldDir = _float3(0.f);
	_float	lifeTime= 0.f;
	_int	alive =0;
	_float2	size = _float2(0.f);
	_float padding;
	_float4 randTheta = _float4(0.f);
	_float4 nowMovingDir = _float4(0.f);
};

struct ParticleSystemDesc
{
	_float				m_fDuration = 5.f;

	_float				m_fCreateInterval = 0.005f;
	
	_uint				m_iMaxParticle = 1000;
	_float				m_fAccTime = 0.f;
	_float				m_fMinLifeTime = 0.5f;
	_float				m_fMaxLifeTime = 1.f;
	_float				m_fMinSpeed = 2;
	_float				m_fMaxSpeed = 5;
	
	_float				m_fStartScale = 10.f;
	_float				m_fEndScale = 5.f;

	_float				m_fCreatingTime = 1.f;
};

struct ComputeSharedInfo
{
	int addCount = 0;
	_float3 padding;
};

class StructuredBuffer;

class ParticleSystem :
    public Component
{
public:
	enum Pass_Info
	{
		Particle_CS =0,
		Particle_Normal = 1,
		Particle_Gradation = 2
	};

public:
    ParticleSystem(shared_ptr<Shader> shader);
    virtual ~ParticleSystem();

public:
	virtual HRESULT Init() override;
    virtual void Tick() override;

    virtual void Final_Tick() override;

    void Render();
	void Create_Buffer();

	void Set_Mesh(shared_ptr<Mesh> mesh) { m_pMesh = mesh; }
	void Set_Material(shared_ptr<Material> material);
	void Set_ComputeMaterial(shared_ptr<Material> computeMaterial);
	shared_ptr<Mesh> Get_Mesh() { return m_pMesh; }
	shared_ptr<Material> Get_Material() { return  m_pMaterial; }
	shared_ptr<Material> Get_ComputeMaterial() { return  m_pComputeMaterial; }

	void Set_Pass(_uint index) { m_iPass = index; }
	void Set_CSPass(_uint index) { m_iCSPass = index; }
	_uint Get_Pass() { return m_iPass; }
	_uint Get_CSPass() { return m_iCSPass; }
	shared_ptr<Shader> Get_Shader() { return m_pShader; }
	ParticleSystemDesc& Get_ParticleSystemDesc() { return m_ParticleSystemDesc; }
	EffectDesc& Get_EffectDesc() { return m_EffectDesc; }
	void Set_TurnState(_bool flag) { m_bTurnOn = flag; Restart_System(); }
	void Restart_System();
	shared_ptr<Model> Get_Model() { return m_pModel; }
	void Set_Model(shared_ptr<Model> model);

private:
	EffectDesc			m_EffectDesc;
private:

    shared_ptr<StructuredBuffer> m_pParticleBuffer = nullptr;
    shared_ptr<StructuredBuffer> m_pComputeSharedBuffer = nullptr;

	shared_ptr<Shader> m_pShader;
	shared_ptr<Mesh> m_pMesh;
	shared_ptr<Material> m_pMaterial;
	shared_ptr<Material> m_pComputeMaterial;

	shared_ptr<Model> m_pModel;


	_uint m_iPass = 0;
	_uint m_iCSPass = Particle_CS;
	_float m_fSystemAge =0.f;
	_bool m_bTurnOn = false;

	ParticleSystemDesc m_ParticleSystemDesc;

public:
	auto& Get_ComputeParamDesc() { return m_ComputeParams; }
	auto& Get_RenderParamDesc() { return m_RenderParams; }

private:
	RenderParams m_ComputeParams{};
	RenderParams m_RenderParams{};
};

