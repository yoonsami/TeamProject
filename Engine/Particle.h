#pragma once
#include "Component.h"

struct ParticleInfo_UAV // 주의: Shader_Particle2의 ParticleInfo_UAV와 대응해야함.
{
    _float fCurrAge;
    _float3 vCurrWorldPos;

    _float  fLifeTime;
    _float3 vRotationAngle;
    
    _int iIsAlive;
    _float3 vRotationSpeed;
    
    _float2 StartEndScale;
    _float2 StartEndSpeed;
    
    _float4 vDiffuseColor;
    
    _float3 vCurrWorldDir;
    _float  Padding;
};  

struct ComputeShared_UAV // 주의: Shader_Particle2의 ComputeShared_UAV와 대응해야함.
{
    int     addCount;
    _float3 padding;
};

class StructuredBuffer;
class Particle : public Component
{
public:
    enum TYPE   {TYPE_Mesh, TYPE_Model, TYPE_END};
    enum TECHNIQUE {TECHNIQUE_Compute, TECHNIQUE_MeshRender, TECHNIQUE_ModelRender};   // Shader_Particle2.fx의 technique
    enum COMPUTE_PASS { CP_Default, CP_END };
    enum RENDERMESH_PASS { RP_Default, RP_Full_Billbord, RP_Horizontal_Billbord, RP_Vertical_Billbord, RP_END };

    typedef struct tagParticle
    {
        // Textures
        string  strSelected_Texture_Shape = { "None" };
        string  strSelected_Texture_Dissolve = { "None" };
        string  strSelected_Texture_Option1 = { "None" };
        string  strSelected_Texture_Option2 = { "None" };

        // Diffuse Color
        _int    iColorOption = { 0 };
        Color   vStartColor;
        Color   vEndColor;
        Color   vDestColor;         

        // Alpha gradation
        _float	fGradationByAlpha_Brighter = { 0.f };
        _float	fGradationByAlpha_Darker = { 0.f };
        _bool   bUseFadeOut = { false };
        
        // Duration ( Particle GameObject의 lifetime )
        _float  fDuration;

        // Create Interval
        _bool   bIsbCreateOnce;
        _float  fCreateInterval;

        // Particle Count
        _int    iMinCnt = { 1 };
        _int    iMaxCnt = { 10 };
        _int    iMaxInstanceCnt = { 10 };
        
        // LifeTime
        _float2 vLifeTime;             

        // Billbord
        _int    iBillbordOption;

        // Movement 
        _int    iMovementOption;
        _float4 vMovementOffsets;

        // Speed
        _float2 vStartSpeed;          
        _float2 vEndSpeed;
        _int    iEndSpeedOption;     

        // Scale
        _float2 vStartScale;
        _float2 vEndScale;
        _int    iEndScaleOption;

        // Create Particle Position 
        _float3	vCenterPosition = { 0.f, 0.f, 0.f };
        _float3	vCreateRange = { 1.f, 1.f, 1.f };
        _float4 vCreateOffsets = { 0.f, 0.f, 0.f, 0.f };  

        // Rotation Speed, Angle 
        _float2 vRotationSpeed_X;
        _float2 vRotationSpeed_Y;
        _float2 vRotationSpeed_Z;
        _float2 vRotationAngle_X;
        _float2 vRotationAngle_Y;
        _float2 vRotationAngle_Z;
    }DESC;

public:
    Particle(shared_ptr<Shader> shader);
    virtual ~Particle();

public:
    virtual HRESULT Init(void* pArg);
    virtual void    Tick() override;
    virtual void    Final_Tick() override;  // CS
    void            Render();               // VS, PS

private:
    void            Init_ComputeParams();
    void            Init_RenderParams();
    void            Init_CreateParticleParams();

    void            Bind_BasicData_ToShader();
    void            Bind_ComputeParams_ToShader();
    void            Bind_RenderParams_ToShader();
    void            Bind_CreateParticleParams_ToShader();

private:
    /* Particle GameObject's Data */
    DESC                    m_tDesc;
    
    _float                  m_fCurrAge = { 0.f };
    _float                  m_fTimeAcc_CreatCoolTime = { 0.f };
    _bool                   m_bIsFirstCreateParticleDone = { false };
 
    TYPE                    m_eType = {TYPE_END};           
    RENDERMESH_PASS         m_eRenderPass = { RP_Default }; 
    COMPUTE_PASS            m_eComputePass = { CP_Default };

    /* Data to bind or get Shader */
    shared_ptr<Shader>      m_pShader = { nullptr };     
    shared_ptr<Mesh>        m_pMesh = { nullptr };       
    shared_ptr<Model>       m_pModel = { nullptr };      
    shared_ptr<Material>    m_pMaterial = { nullptr };   // For. Bind texture 

    RenderParams            m_ComputeParams{};               
    RenderParams            m_RenderParams{};            
    CreateParticleDesc      m_CreateParticleParams;     

    shared_ptr<StructuredBuffer> m_pParticleInfo_UAVBuffer = nullptr;   
    shared_ptr<StructuredBuffer> m_pComputeShared_UAVBuffer = nullptr;  
};

