#pragma once

struct ModelMesh;

enum class PARTS_INFO
{
	Hair,
	Head,
	Uniform,
	BackParts,
	NonParts,
	END

};

enum
{
	PARTS_MAX_COUNT = _uint(PARTS_INFO::END)
};

struct  Parts
{
public:
	~Parts();
	
	shared_ptr<Parts> Clone();
	vector<shared_ptr<ModelMesh>> m_Meshes;

	vector<shared_ptr<ModelMesh>>& Get_Meshes() { return m_Meshes; }
	shared_ptr<ModelMesh> Get_MeshByIndex(_int index) { return(index < 0 || index >= m_Meshes.size() ? nullptr : m_Meshes[index]); }
	shared_ptr<ModelMesh> Get_MeshByName(const wstring& meshName);

};

