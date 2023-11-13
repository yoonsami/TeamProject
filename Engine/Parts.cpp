#include "pch.h"
#include "Parts.h"
#include "ModelMesh.h"

Parts::~Parts()
{

}

shared_ptr<Parts> Parts::Clone()
{
	shared_ptr<Parts> clone = make_shared<Parts>();

	clone->m_Meshes.reserve(m_Meshes.size());

	for (auto& mesh : m_Meshes)
	{
		clone->m_Meshes.push_back(mesh->Clone());

	}
	return clone;
}

shared_ptr<ModelMesh> Parts::Get_MeshByName(const wstring& meshName)
{
	for (auto& mesh : m_Meshes)
	{
		if (mesh->name == meshName)
			return mesh;
	}
	return nullptr;
}
