#include "pch.h"
#include "Mesh.h"

#include "GeometryHelper.h"

Mesh::Mesh()
	: ResourceBase(ResourceType::Mesh)
{
	m_pGeometry = make_shared<Geometry<VTXTEXNORTANDATA>>();
}

Mesh::~Mesh()
{
}

void Mesh::CreateQuad()
{
	
	GeometryHelper::CreateQuad(m_pGeometry);
	Create_Buffer();

}

void Mesh::CreateCube()
{
	
	GeometryHelper::CreateCube(m_pGeometry);
	Create_Buffer();
}

void Mesh::CreateGrid(_uint sizeX, _uint sizeZ)
{
	
	GeometryHelper::CreateGrid(m_pGeometry, sizeX, sizeZ);
	Create_Buffer();
}

void Mesh::CreateSphere()
{

	GeometryHelper::CreateSphere(m_pGeometry);
	Create_Buffer();
}

void Mesh::CreatePoint()
{
	GeometryHelper::CreatePoint(m_pGeometry);
	Create_Buffer();
}

void Mesh::Create_Buffer()
{
	m_pVB = make_shared<VertexBuffer>();
	m_pVB->Create(m_pGeometry->Get_Vertices());

	m_pIB = make_shared<IndexBuffer>();
	m_pIB->Create(m_pGeometry->Get_Indices());
}
