#pragma once
#include "Parts.h"
#include "ResourceBase.h"

struct ModelBone;
struct ModelMesh;
struct ModelAnimation;
struct AnimTransform
{
	using TransformArrayType = array<_float4x4, MAX_MODEL_TRANSFORMS>;
	vector<TransformArrayType> transforms;
};
class Model : public ResourceBase
{
public:
	Model();
	~Model();

public:
	void ReadMaterial(const wstring& strPath);
	void ReadModel(const wstring& strPath);
	void ReadAnimation(const wstring& strPath);

	void AddParts(const wstring& partName, PARTS_INFO partType);

	shared_ptr<ResourceBase> Clone() override;
	array<shared_ptr<Parts>, PARTS_MAX_COUNT>& Get_Parts() { return m_Parts; }
private:
	void Bind_CacheInfo();
	
public:
	ComPtr<ID3D11ShaderResourceView> Get_TransformSRV() { return m_pSRV; }

	wstring Get_ModelTag() { return m_ModelTag; }
	wstring Get_MaterialTag() { return m_MaterialTag; }
	wstring Get_AnimationTag() { return m_AnimationTag; }


	_uint Get_MaterialCount() { return static_cast<_uint>(m_Materials.size()); }
	vector<shared_ptr<Material>>& Get_Materials() { return m_Materials; }
	shared_ptr<Material> Get_MaterialByIndex(_int index) { return (index < 0 || index >= m_Materials.size() ? nullptr : m_Materials[index]); }
	shared_ptr<Material> Get_MaterialByName(const wstring& materialName);

	_uint Get_MeshCount();
	vector<shared_ptr<ModelMesh>>& Get_Meshes() { return m_Meshes; }
	shared_ptr<ModelMesh> Get_MeshByIndex(_int index) { return(index < 0 || index >= m_Meshes.size() ? nullptr : m_Meshes[index]); }
	shared_ptr<ModelMesh> Get_MeshByName(const wstring& meshName);

	_uint Get_PartsMeshCount();
	vector<shared_ptr<ModelMesh>> Get_PartsMeshes();
	shared_ptr<ModelMesh> Get_PartsMeshByName(const wstring& meshName);

	_uint Get_BoneCount() { return static_cast<_uint>(m_Bones.size()); }
	vector<shared_ptr<ModelBone>>& Get_Bones() { return m_Bones; }
	shared_ptr<ModelBone> Get_BoneByIndex(_int index) { return (index < 0 || index >= m_Bones.size() ? nullptr : m_Bones[index]); }
	shared_ptr<ModelBone> Get_BoneByName(const wstring& boneName);
	shared_ptr<ModelBone> Get_RootBone() { return m_pRootBone; }
	_int Get_BoneIndexByName(const wstring& boneName);

	_uint Get_AnimationCount() { return static_cast<_uint>(m_Animations.size()); }
	vector<shared_ptr<ModelAnimation>>& Get_Animations() { return m_Animations; }
	shared_ptr<ModelAnimation> Get_AnimationByIndex(_int index) { return (index < 0 || index >= m_Animations.size() ? nullptr : m_Animations[index]); }
	shared_ptr<ModelAnimation> Get_AnimationByName(const wstring& animName, _int* pIndex = nullptr);
	AnimAddonDesc& Get_AnimAddonDesc() { return m_BoneIndexDesc; }
	_float4x4 Get_AnimationTransform(_int animIndex, _int frame, _int boneIndex)
	{
		return m_AnimTransforms[animIndex].transforms[frame][boneIndex];

	}
	vector<vector<_float3>>& Get_RootBonePosition() { return m_RootBonePosition; }
	_bool Has_Parts() { return m_bHasParts; }

private:
	void Create_Texture();
	void Create_BoneData();
	void Create_AnimationTransform(_uint index);
	
private:
	wstring m_strModelPath = L"../Resources/Models/";
	wstring m_strTexturePath = L"../Resources/Textures/";
	wstring m_ModelTag = L"";
	wstring m_MaterialTag = L"";
	wstring m_AnimationTag = L"";



private:
	vector<AnimTransform> m_AnimTransforms;
	vector<vector<_float3>> m_RootBonePosition;
	ComPtr<ID3D11Texture2D> m_pTexture;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;

	AnimAddonDesc m_BoneIndexDesc;
	
private:
	_bool m_bHasParts = false;
	shared_ptr<ModelBone> m_pRootBone;
	vector<shared_ptr<Material>> m_Materials;
	vector<shared_ptr<ModelBone>> m_Bones;
	vector<shared_ptr<ModelMesh>> m_Meshes;
	vector<shared_ptr<ModelAnimation>> m_Animations;
	array<shared_ptr<Parts>, PARTS_MAX_COUNT> m_Parts;

};

