#include "pch.h"
#include "Particle.h"

#include "Camera.h"
#include "StructuredBuffer.h"

Particle::Particle(shared_ptr<Shader> shader)
	: Component(COMPONENT_TYPE::ParticleSystem)
	, m_pShader(shader)
{
}

Particle::~Particle()
{
}

HRESULT Particle::Init(void* tDesc)
{
	// For. Setting basic info  
	memcpy(&m_tDesc, tDesc, sizeof(tDesc));

	if (!m_pModel) 
		m_eType = TYPE_Mesh;
	else 
		m_eType = TYPE_Model;

	m_eComputePass = (COMPUTE_PASS)m_tDesc.iMovementOption;
	m_eRenderPass = (RENDERMESH_PASS)m_tDesc.iBillbordOption;

	// For. Create ParticleInfo UAV Buffers  
	vector<ParticleInfo_UAV> tmp(m_tDesc.iMaxParticleNum);
	if (!m_pParticleInfo_UAVBuffer)
		m_pParticleInfo_UAVBuffer = make_shared<StructuredBuffer>(tmp.data(), static_cast<_uint>(sizeof ParticleInfo_UAV), m_tDesc.iMaxParticleNum);

	// For. Create ComputeShader UAV Buffers  
	if (!m_pComputeShared_UAVBuffer)
		m_pComputeShared_UAVBuffer = make_shared<StructuredBuffer>(nullptr, static_cast <_uint>(sizeof m_pComputeShared_UAVBuffer), 1);

	return S_OK;
}

void Particle::Tick()
{
}

void Particle::Final_Tick()
{
	// For. Update TimeAcc 
	m_fCurrLifeTime += fDT;
	m_fTimeAcc_CreatCoolTime += fDT;

	// For. Check End(Delete) this particle 
	if (m_fCurrLifeTime >= m_tDesc.fDuration)
	{
		CUR_SCENE->Remove_GameObject(Get_Owner());
		return;
	}

	// For. Decide whether to create a new Particle and Cnt
	_int iAddCnt = 0; // 이번 틱에 새로 만들어야할 파티클개수 
	if (m_fTimeAcc_CreatCoolTime > m_tDesc.fCreateInterval)
	{
		// min ~ max 사이의 랜덤한 수를 골라 그만큼 새로 생성한다. 
		iAddCnt = (rand() % (m_tDesc.iMaxCnt + 1 - m_tDesc.iMinCnt)) + m_tDesc.iMinCnt;
		m_fTimeAcc_CreatCoolTime = 0.f;
	}
	
	// For. Bind Data in shader ( 쉐이더가 값을 보관할 수 없기 때문에 매 프레임마다 넘겨줘야한다.)
	auto& world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	Bind_ComputeShaderData_ToShader();

	// For. Bind UAV Buffer in shader ( 쉐이더가 값을 보관할 수 없기 때문에 매 프레임마다 넘겨줘야한다.)
	m_pShader->GetUAV("g_ParticleInfo_UAVBuffer")->SetUnorderedAccessView(m_pParticleInfo_UAVBuffer->Get_UAV().Get());
	m_pShader->GetUAV("g_ComputeShared_UAVBuffer")->SetUnorderedAccessView(m_pComputeShared_UAVBuffer->Get_UAV().Get());	 
}

void Particle::Render()
{
	/* 여기서 Shader에 바인딩하는 값들은 VS, PS를 돌리기 위해 사용되는 값들이다. */

	// For. Render Standard Mesh
	switch (m_eType)
	{
	case TYPE_Mesh:
		if (nullptr == m_pMesh || nullptr == m_pMaterial)
			return;

		// For. Bind data to shader
		Bind_BasicData_ToShader();
		Bind_RenderShaderData_ToShader();
		
		m_pMesh->Get_VertexBuffer()->Push_Data();
		m_pMesh->Get_IndexBuffer()->Push_Data();
		
		// For. Setting Context Topology
		CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		
		// For. Draw call
		m_pShader->DrawIndexedInstanced(TECHNIQUE_MeshRender, m_eRenderPass, m_pMesh->Get_IndexBuffer()->Get_IndicesNum(), m_tDesc.iMaxParticleNum);
		break;

	case TYPE_Model:
		if (nullptr == m_pModel)
			return;
		
		// For. Bind data to shader
		Bind_BasicData_ToShader();
		Bind_RenderShaderData_ToShader();

		// TODO 

		break;
	}
}

void Particle::Bind_BasicData_ToShader()
{
	/* Particle 이 mesh든 model이든 상관없이 무조건 쉐이더에 바인딩해야하는 데이터들 */

	m_pShader->GetSRV("g_Data")->SetResource(m_pParticleInfo_UAVBuffer->Get_SRV().Get());

	auto world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());
}

void Particle::Bind_ComputeShaderData_ToShader()
{

}

void Particle::Bind_RenderShaderData_ToShader()
{

}
