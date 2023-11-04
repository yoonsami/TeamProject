#pragma once
#include "Component.h"

struct ParticleInfo_UAV // 주의: Shader_Particle2의 ParticleInfo_UAV와 대응해야함.
{
    /* 매 프레임마다 변할 수 있는 값들 */
    _float3  vWorldPos;      // 입자의 현재 World pos 
    _float   fCurrTime;      // 입자가 생성되고나서부터 얼마나 시간이 흘렀는지
    _float3  vWorldDir;      // 입자의 현재 방향 
    _int     iAlive;         // 입자가 현재 살아있는가. 
    _float4  vColor;         // 입자의 color 
    _float   fSpeed;         // 입자의 speed
    _float   fSpeeeExponent; // 입자의 speed Exponent

    /* 입자가 처음 생성될 때 고정되는 값들 */
    _float2  vSize;          // 입자의 size 
    _float   fLifeTime;      // 입자의 LifeTime 

    /* 자유롭게 사용한 데이터 공간 */
    _float3  vOffsets;       // 16 byte맞추기 위해 넣음. 쓸일있으면 알아서 맞춰서 쓰면됨. 
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
        string  m_strSelected_Texture_Dissolve = { "None" };
        string  m_strSelected_Texture_Distortion = { "None" };
        string  strSelected_Texture_Option1 = { "None" };
        string  strSelected_Texture_Option2 = { "None" };

        // Diffuse Color 
        _int    iSelected_ColorOption;
        Color   vConstantColor;
        Color   vStartColor;
        Color   vEndColor;

        // Coloring Option 
        _bool	bGradationByAlpha_Brighter = { false };
        _bool	bGradationByAlpha_Darker = { false };
        _bool   bDissolveOn = { false };
        _float  fDissolveSpeed = { 1.f };
        _bool   bDistortionOn = { false };
        _float  fDistortionOffset[2] = { 0.5f, 0.5f };

        // Duration ( 본 Particle GameObject의 lifetime )
        _float  fDuration;

        // Create Interval
        _float  fCreateInterval;

        // Particle Count
        _int    iMinCnt = { 1 };
        _int    iMaxCnt = { 10 };
        _int    iMaxParticleNum = { 10 };

        // LifeTime ( Particle 하나의 lifetime )
        _int    iLifeTimeOption;
        _float2 vLifeTime;

        // Speed
        _int    iSpeedOption;
        _float2 vSpeed;

        // Billbord
        _int    iBillbordOption;

        // Movement 
        _int    iMovementOption;
        _float4 vMovementOffsets;

        // IsLoop
        _bool   bIsLoop;

        // Create Particle Position 
        _float	fCenterPosition[3] = { 0.f, 0.f, 0.f };
        _float	fCreateRange[3] = { 1.f, 1.f, 1.f };

        // Scale
        _float2 vScale = { 1.f, 5.f };

        // Rotation 
        _float3 fRotationSpeed = { 0.f, 0.f, 0.f };
        _float3 fRotationDir = { 1.f, 1.f, 1.f };
    }DESC;

public:
    Particle(shared_ptr<Shader> shader);
    virtual ~Particle();

public:
    virtual HRESULT Init(void* tDesc);      //씬 로딩시에 추가된 컴포넌트들의 init은 씬 init에서 자동으로 불리고, 중간에 추가된 것들은 직접 호출해야됨. 
    virtual void    Tick() override;
    virtual void    Final_Tick() override;  // CS
    void            Render();               // VS, PS

    /* Setter */
    

    /* Getter */
    auto&           Get_ComputeParamDesc() { return m_ComputeParams; }
    auto&           Get_RenderParamDesc() { return m_RenderParams; }

private:
    void            Bind_BasicData_ToShader();
    void            Bind_ComputeShaderData_ToShader();
    void            Bind_RenderShaderData_ToShader();

private:
    DESC                    m_tDesc;
    
    TYPE                    m_eType = {TYPE_END};           // 본 Particle이 standard mesh인지, 모델인지
    RENDERMESH_PASS         m_eRenderPass = { RP_Default }; // 본 Particl이 랜더할 때 사용할 pass (VS, PS)
    COMPUTE_PASS            m_eComputePass = { CP_Default };// 본 Particl이 쉐이더에서 연산할 때 사용할 pass (CS)

    shared_ptr<Shader>      m_pShader = { nullptr };     // 본 Particle의 Render에서 사용할 shader
    shared_ptr<Mesh>        m_pMesh = { nullptr };       // 본 Particle의 Render에서 그릴 Mesh
    shared_ptr<Model>       m_pModel = { nullptr };      // 본 Particle의 Render에서 그릴 Model
    shared_ptr<Material>    m_pMaterial = { nullptr };   // 본 Particle이 사용하는 Textures, Colors를 Tick에서 shader로 바인드함. 

    RenderParams            m_ComputeParams{};      // Computer shader에서 사용하는 정보들을 담은 구조체 (Render()에서 쉐이더로 넘기기)
    RenderParams            m_RenderParams{};       // VS, PS에서 사용하는 정보들을 담은 구조체 (Render()에서 쉐이더로 넘기기)

    shared_ptr<StructuredBuffer> m_pParticleInfo_UAVBuffer = nullptr;   // 쉐이더와 데이터를 주고받는 UAV (입자 하나하나에 대한 정보)
    shared_ptr<StructuredBuffer> m_pComputeShared_UAVBuffer = nullptr;  // 쉐이더와 데이터를 주고받는 UAV

    /* Time Acc */
    _float                  m_fCurrLifeTime = { 0.f };   // 본 Particle이 생성된 후 흐른 시간 
    _float                  m_fTimeAcc_CreatCoolTime = { 0.f };
};

