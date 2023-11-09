#include "pch.h"
#include "TrailRenderer.h"

#include "Camera.h"
#include "Material.h"

TrailRenderer::TrailRenderer(shared_ptr<Shader> shader,_int iMaxTrailCount, _float fTrailSize)
	: Component(COMPONENT_TYPE::TrailRenderer)
	, m_pShader(shader)
	, m_iMaxTrailCount(iMaxTrailCount)
	, m_fTrailSize(fTrailSize)
{
	m_vVertexDir.Normalize();
	vector<VTXTEXNORTANDATA> vertices(m_iMaxTrailCount * 4);
	m_Indices.resize(m_iMaxTrailCount * 6);

	for (_int i = 0; i < m_iMaxTrailCount; ++i)
	{
		m_Indices[i * 6 + 0] = i * 4 + 0;
		m_Indices[i * 6 + 1] = i * 4 + 1;
		m_Indices[i * 6 + 2] = i * 4 + 2;
		m_Indices[i * 6 + 3] = i * 4 + 0;
		m_Indices[i * 6 + 4] = i * 4 + 2;
		m_Indices[i * 6 + 5] = i * 4 + 3;
	}

	m_pVB = make_shared<VertexBuffer>();
	m_pVB->Create(vertices, 0, true);

	m_pIB = make_shared<IndexBuffer>();
	m_pIB->Create(m_Indices);
}

TrailRenderer::~TrailRenderer()
{
}

void TrailRenderer::Final_Tick()
{
	
	
	if (!m_bRunning)
		return;
	m_fAccTime += fDT;
	if (m_fCreateInterval <= m_fAccTime)
	{
		m_fAccTime -= m_fCreateInterval;
		
		_float3 centerPos = m_vCenterPos;

	/*	if (m_pMaterial->Get_RenderParamDesc().intParams[0] == 0)
		{
			centerPos = _float3(0.f);
		}*/

		if (m_iTrailCount == 0)
		{
			{
				VTXTEXNORTANDATA vtx;
				vtx.vPosition = centerPos + m_vVertexDir * m_fTrailSize * 0.5f * -1.f;
				vtx.vTexCoord = _float2(0.f, 1.f);
				m_Vertices.push_back(vtx);
			}
			{
				VTXTEXNORTANDATA vtx;
				vtx.vPosition = centerPos + m_vVertexDir * m_fTrailSize * 0.5f * 1.f;
				vtx.vTexCoord = _float2(1.f, 1.f);
				m_Vertices.push_back(vtx);
			}
			{
				VTXTEXNORTANDATA vtx;
				vtx.vPosition = centerPos + m_vVertexDir * m_fTrailSize * 0.5f * 1.f;
				vtx.vTexCoord = _float2(1.f, 1.f);
				m_Vertices.push_back(vtx);
			}
			{
				VTXTEXNORTANDATA vtx;
				vtx.vPosition = centerPos + m_vVertexDir * m_fTrailSize * 0.5f * -1.f;
				vtx.vTexCoord = _float2(0.f, 1.f);
				m_Vertices.push_back(vtx);
			}
			m_iTrailCount++;
		}
		else if (m_iTrailCount < m_iMaxTrailCount)
		{
			auto iter = m_Vertices.end();
			_float3 preCenterPos = ((*--iter).vPosition + (*--iter).vPosition) * 0.5f;

			_float3 vDir = centerPos - preCenterPos;
			vDir.Normalize();
			_float3 vCameraLook = CUR_SCENE->Get_MainCamera()->Get_Transform()->Get_State(Transform_State::LOOK).xyz();
			_float3 vCameraRight = CUR_SCENE->Get_MainCamera()->Get_Transform()->Get_State(Transform_State::RIGHT).xyz();

			_float3 vFinalDir;
			if (vDir.Dot(vCameraLook) > cosf(XM_PI / 18.f))
				vFinalDir = vCameraRight;
			else
				vFinalDir = vDir.Cross(vCameraLook);


			VTXTEXNORTANDATA vtx0;
			vtx0.vPosition = centerPos + vFinalDir * m_fTrailSize * 0.5f * -1.f;
			vtx0.vTexCoord = _float2(0.f, 1.f - _float(m_iTrailCount) / 5.f);

			VTXTEXNORTANDATA vtx1;
			vtx1.vPosition = centerPos + vFinalDir * m_fTrailSize * 0.5f * 1.f;
			vtx1.vTexCoord = _float2(1.f, 1.f - _float(m_iTrailCount) / 5.f);


			VTXTEXNORTANDATA vtx2;
			vtx2.vPosition = (*--iter).vPosition;
			vtx2.vTexCoord = _float2(1.f, 1.f - _float(m_iTrailCount - 1) / 5.f);

			VTXTEXNORTANDATA vtx3;
			vtx3.vPosition = (*--iter).vPosition;
			vtx3.vTexCoord = _float2(0.f, 1.f - _float(m_iTrailCount - 1) / 5.f);

			m_Vertices.push_back(vtx0);
			m_Vertices.push_back(vtx1);
			m_Vertices.push_back(vtx2);
			m_Vertices.push_back(vtx3);

			m_iTrailCount++;
		}
		else
		{
			auto iter = m_Vertices.end();
			_float3 preCenterPos = ((*--iter).vPosition + (*--iter).vPosition) * 0.5f;

			_float3 vDir = centerPos - preCenterPos;
			vDir.Normalize();
			_float3 vCameraLook = CUR_SCENE->Get_MainCamera()->Get_Transform()->Get_State(Transform_State::LOOK).xyz();
			_float3 vCameraRight = CUR_SCENE->Get_MainCamera()->Get_Transform()->Get_State(Transform_State::RIGHT).xyz();
			
			_float3 vFinalDir;
			if (vDir.Dot(vCameraLook) > cosf(XM_PI / 18.f))
				vFinalDir = vCameraRight;
			else
				vFinalDir = vDir.Cross(vCameraLook);

			VTXTEXNORTANDATA vtx0;
			vtx0.vPosition = centerPos + vFinalDir * m_fTrailSize * 0.5f * -1.f;
			vtx0.vTexCoord = _float2(0.f, 1.f - _float(m_iTrailCount) / 5.f);

			VTXTEXNORTANDATA vtx1;
			vtx1.vPosition = centerPos + vFinalDir * m_fTrailSize * 0.5f * 1.f;
			vtx1.vTexCoord = _float2(1.f, 1.f - _float(m_iTrailCount) / 5.f);


			VTXTEXNORTANDATA vtx2;
			vtx2.vPosition = (*--iter).vPosition;
			vtx2.vTexCoord = _float2(1.f, 1.f - _float(m_iTrailCount - 1) / 5.f);

			VTXTEXNORTANDATA vtx3;
			vtx3.vPosition = (*--iter).vPosition;
			vtx3.vTexCoord = _float2(0.f, 1.f - _float(m_iTrailCount - 1) / 5.f);

			m_Vertices.push_back(vtx0);
			m_Vertices.push_back(vtx1);
			m_Vertices.push_back(vtx2);
			m_Vertices.push_back(vtx3);

			m_Vertices.pop_front();
			m_Vertices.pop_front();
			m_Vertices.pop_front();
			m_Vertices.pop_front();
		}

		vector<VTXTEXNORTANDATA> vertices;
		vertices.reserve(m_iMaxTrailCount * 4);
		for (auto& vtx : m_Vertices)
			vertices.push_back(vtx);

		D3D11_MAPPED_SUBRESOURCE subResource;
		CONTEXT->Map(m_pVB->Get_ComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, vertices.data(), sizeof(VTXTEXNORTANDATA) * m_Vertices.size());
		}
		CONTEXT->Unmap(m_pVB->Get_ComPtr().Get(), 0);


	}
}

void TrailRenderer::Render()
{
	if (!m_bRunning)
		return;
	auto world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	m_pMaterial->Tick();

	m_pShader->Push_RenderParamData(m_RenderParams);

	m_pVB->Push_Data();
	m_pIB->Push_Data();

	if(m_iTrailCount < m_iMaxTrailCount)
		m_pShader->DrawIndexed(0, m_iPass, m_iTrailCount * 6);
	else if(m_iTrailCount >= m_iMaxTrailCount)
		m_pShader->DrawIndexed(0, m_iPass, m_iMaxTrailCount * 6);
}

void TrailRenderer::Reset()
{
	m_iTrailCount = 0;
	m_Vertices.clear();
}

void TrailRenderer::Set_Material(shared_ptr<Material> material)
{
	m_pMaterial = material;
	m_pMaterial->Set_Shader(m_pShader);
}
