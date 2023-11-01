#include "pch.h"
#include "BaseCollider.h"

BaseCollider::BaseCollider(ColliderType colliderType)
	:Component(COMPONENT_TYPE::Collider), m_eColliderType(colliderType)
{
	//DebugShader =  RESOURCES.Get<Shader>(L"Shader_Mesh.fx");
	m_iID = g_iNextID++;
}

BaseCollider::~BaseCollider()
{
}

void BaseCollider::Render()
{
	/*auto pCamera = CUR_SCENE->Get_Camera(L"Default")->Get_Camera();

	if (!pCamera)
		return;

	Color drawColor;
	if (m_iColCount > 0)
		drawColor = Color(1.f, 0.f, 0.f, 1.f);
	else
		drawColor = Color(0.f, 1.f, 0.f, 1.f);

	if(!m_bActivated)
		drawColor = Color(0.f, 0.f, 0.f, 1.f);
	DebugShader->GetVector("g_DrawColor")->SetFloatVector((_float*) & drawColor);
	DebugShader->Push_GlobalData(pCamera->Get_ViewMat(), pCamera->Get_ProjMat());

	DebugShader->Push_TransformData(Get_TransformDesc());

	for (auto& mesh : m_Meshes)
	{
		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DebugShader->DrawIndexed(1, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}*/
}

void BaseCollider::Render_Instancing(shared_ptr<InstancingBuffer>& buffer)
{
//	/*auto pCamera = CUR_SCENE->Get_Camera(L"Default")->Get_Camera();
//
//	if (!pCamera)
//		return;
//
//	DebugShader->Push_GlobalData(pCamera->Get_ViewMat(), pCamera->Get_ProjMat());
//
//	for (auto& mesh : m_Meshes)
//	{
//		mesh->Get_VertexBuffer()->Push_Data();
//		mesh->Get_IndexBuffer()->Push_Data();
//
//		buffer->Push_Data();
//
//		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		DebugShader->DrawIndexedInstanced(0, 3, mesh->Get_IndexBuffer()->Get_IndicesNum(), buffer->Get_Count());
//	}*/
}

void BaseCollider::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
	if (m_pOwner.expired())
		return;
	//auto myFSM = Get_Owner()->Get_FSM();
	//if (myFSM)
	//	myFSM->OnCollision(pCollider, fGap);
}

void BaseCollider::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
	++m_iColCount;
	//if (m_pOwner.expired())
	//	return;
	//auto myFSM = Get_Owner()->Get_FSM();
	//if (myFSM)
	//	myFSM->OnCollisionEnter(pCollider, fGap);

	//if (m_pResultFunc)
	//	m_pResultFunc();

	
}

void BaseCollider::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
	--m_iColCount;
	if (m_iColCount < 0)
		m_iColCount = 0;
	//if (m_pOwner.expired())
	//	return;
	//auto myFSM = Get_Owner()->Get_FSM();
	//if (myFSM)
	//	myFSM->OnCollisionExit(pCollider, fGap);
}

void BaseCollider::Add_ColliderGroup()
{
	if (m_eCollisionGroup < MAX_COLLIDER_GROUP)
		COLLISION.Add_ColliderGroup(m_eCollisionGroup, shared_from_this());
}

_uint BaseCollider::g_iNextID =0;
