#pragma once
#include "Component.h"

struct ParticleInfo_UAV // 주의: Shader_Particle2의 ParticleInfo_UAV와 대응해야함.
{
    /* 매 프레임마다 변할 수 있는 값들 */
    _float   fCurrCurrTime;      // 입자가 현재 생성되고나서부터 얼마나 시간이 흘렀는지
    _float3  vCurrWorldPos;      // 입자의 현재 World pos 
    _float   fCurrSpeed;         // 입자의 현재 speed
    _float3  vCurrRotationSpeed; // 입자의 현재 회전속도 
    _float   fCurrSpeedExponent; // 입자의 현재 speed Exponent
    _float3  vCurrWorldDir;      // 입자의 현재 현재 방향 
    _float4  vCurrColor;         // 입자의 현재 color 
    _float2  vCurrSize;          // 입자의 현재 size 
    _int     iCurrAlive;         // 입자가 현재 살아있는가. 

    /* 입자가 처음 생성될 때 고정되는 값들 */
    _float   fLifeTime;          // 입자의 LifeTime 
    _float3  vRotationAngle;     // 입자의 회전각도  

    /* 자유롭게 사용한 데이터 공간 */
    _float   vOffsets;           // 16 byte맞추기 위해 넣음. 쓸일있으면 알아서 맞춰서 쓰면됨. 
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
        Color   vStartColor;
        Color   vEndColor;
        Color   vDestColor;         

        // Alpha gradation
        _float	fGradationByAlpha_Brighter = { 0.f };
        _float	fGradationByAlpha_Darker = { 0.f };
        _bool   bIsAlphaFollowDuration = { false };
        
        // Dissolve
        _int    iDissolveOption = { 0 };            // non, constant, curve
        _float2 fDissolveCurveSpeed = { 0.f, 0.f }; // (if option is curve) speed base, speed exponent

        // Duration ( Particle GameObject의 lifetime )
        _float  fDuration;

        // Create Interval
        _float  fCreateInterval;

        // Particle Count
        _int    iMinCnt = { 1 };
        _int    iMaxCnt = { 10 };
        _int    iMaxParticleNum = { 10 };
        
        // LifeTime ( Particle 하나의 lifetime )
        _int    iLifeTimeOption;        // rand, curve
        _float2 vLifeTime;              // (if option is rand) min, max / (if option is curve) base, exponent  

        // Speed
        _int    iSpeedOption;           // rand, curve
        _float2 vSpeed;                 // (if option is rand) min, max / (if option is curve)  base, exponent

        // Billbord
        _int    iBillbordOption;

        // Movement 
        _int    iMovementOption;
        _float4 vMovementOffsets;

        // IsLoop
        _bool   bIsLoop;

        // Create Particle Position 
        _float3	vCenterPosition = { 0.f, 0.f, 0.f };
        _float3	vCreateRange = { 1.f, 1.f, 1.f };
        _float4 vCreateOffsets = { 0.f, 0.f, 0.f, 0.f };   // 범위 내 랜덤 그 외의 방법으로 생성 위치를 설정할 경우 사용할 값들. (ex.반지름, 각도 등)

        // Scale
        _float2 vStartScale = { 1.f, 5.f };     // min, max
        _int    iScaleOption;                   // constant, curve
        _float2 vScaleSpeed;                    // (if option is constant) speed, no use / (if option is curve)  base, exponent

        // Rotation Speed, Angle 
        _float3 vRotationSpeed;         // x,y,z  
        _int    iRotationAngleOption;   // const, rand
        _float3 vRotationAngle;         // (if option is const) x,y,z/ (if option is rand) min, max  
    
    }DESC;

public:
    Particle(shared_ptr<Shader> shader);
    virtual ~Particle();

public:
    virtual HRESULT Init(void* tDesc);      
    virtual void    Tick() override;
    virtual void    Final_Tick() override;  // CS
    void            Render();               // VS, PS

    /* Setter */
    void            Set_Mesh(shared_ptr<Mesh> mesh) { m_pMesh = mesh; }
    void            Set_Material(shared_ptr<Material> material) { m_pMaterial = material; }

private:
    void            Init_ComputeParams();
    void            Init_RenderParams();
    void            Init_CreateParticleParams();

    void            Bind_BasicData_ToShader();
    void            Bind_ComputeParams_ToShader();
    void            Bind_RenderParams_ToShader();
    void            Bind_CreateParticleParams_ToShader();

private:
    DESC                    m_tDesc;
    
    TYPE                    m_eType = {TYPE_END};           // 본 Particle이 standard mesh인지, 모델인지
    RENDERMESH_PASS         m_eRenderPass = { RP_Default }; // 본 Particl이 랜더할 때 사용할 pass (VS, PS)
    COMPUTE_PASS            m_eComputePass = { CP_Default };// 본 Particl이 쉐이더에서 연산할 때 사용할 pass (CS)

    shared_ptr<Shader>      m_pShader = { nullptr };     // 본 Particle의 Render에서 사용할 shader
    shared_ptr<Mesh>        m_pMesh = { nullptr };       // 본 Particle의 Render에서 그릴 Mesh
    shared_ptr<Model>       m_pModel = { nullptr };      // 본 Particle의 Render에서 그릴 Model
    
    shared_ptr<Material>    m_pMaterial = { nullptr };   // 본 Particle이 사용하는 Textures, Colors를 Tick에서 shader로 바인드함. 

    RenderParams            m_ComputeParams{};           // Computer shader에서 사용하는 정보들을 담은 구조체 (Bind_ComputeShaderData_ToShader()에서 쉐이더로 넘기기)
    RenderParams            m_RenderParams{};            // GS, VS, PS에서 사용하는 정보들을 담은 구조체 (Bind_RenderShaderData_ToShader()에서 쉐이더로 넘기기)
    CreateParticleDesc      m_CreateParticleParams;     

    shared_ptr<StructuredBuffer> m_pParticleInfo_UAVBuffer = nullptr;   // 쉐이더와 데이터를 주고받는 UAV (입자 하나하나에 대한 정보)
    shared_ptr<StructuredBuffer> m_pComputeShared_UAVBuffer = nullptr;  // 쉐이더와 데이터를 주고받는 UAV

    /* Time Acc */
    _float                  m_fCurrLifeTime = { 0.f };   // 본 Particle이 생성된 후 흐른 시간 
    _float                  m_fTimeAcc_CreatCoolTime = { 0.f };
};

