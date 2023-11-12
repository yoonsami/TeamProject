#include "pch.h"
#include "Model.h"

#include "Utils.h"
#include "FileUtils.h"
#include "Material.h"
#include "ModelMesh.h"
#include "Geometry.h"
#include "ModelAnimation.h"

#include <filesystem>
namespace fs = std::filesystem;


Model::Model()
	:ResourceBase(ResourceType::Model)
{

}

Model::~Model()
{
}

void Model::ReadMaterial(const wstring& strPath)
{
	m_MaterialTag = fs::path(strPath).filename().wstring();
	wstring fullPath = strPath + L".Material";

	wstring texturePath = fs::path(strPath).parent_path();
	Utils::Replace(texturePath, L"Models", L"Textures");

	shared_ptr<FileUtils> file = make_shared<FileUtils>();

	file->Open(fullPath, FileMode::Read);
	_uint iCount = file->Read<_uint>();
	for(_uint i = 0; i< iCount; ++i)
	{
		shared_ptr<Material> material = make_shared<Material>();

		material->Set_Name(Utils::ToWString(file->Read<string>()));

		if (file->Read<_bool>())
		{
			wstring textureStr = Utils::ToWString(file->Read<string>());
			Utils::DetachExt(textureStr);
			if (textureStr.length() > 0)
			{
				textureStr += L".dds";
				auto texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);
				if (!texture)
				{
					Utils::ChangeExt(textureStr, L".DDS");
					texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);
					if (!texture)
					{
						Utils::ChangeExt(textureStr, L".tga");
						texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);
						if (!texture)
						{
							Utils::ChangeExt(textureStr, L".png");
							texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);

						}
					}
				}
				material->Set_TextureMap(texture, TextureMapType::DIFFUSE);
			}
		}

		// Specular Texture
		if (file->Read<_bool>())
		{
			wstring textureStr = Utils::ToWString(file->Read<string>());
			Utils::DetachExt(textureStr);
			if (textureStr.length() > 0)
			{
				textureStr += L".dds";
				auto texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);
				if (!texture)
				{
					Utils::ChangeExt(textureStr, L".DDS");
					texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);
					if (!texture)
					{
						Utils::ChangeExt(textureStr, L".tga");
						texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);
						if (!texture)
						{
							Utils::ChangeExt(textureStr, L".png");
							texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);

						}
					}
				}
				material->Set_TextureMap(texture, TextureMapType::SPECULAR);
			}
		}

		// Normal Texture
		if (file->Read<_bool>())
		{
			wstring textureStr = Utils::ToWString(file->Read<string>());
			Utils::DetachExt(textureStr);
			if (textureStr.length() > 0)
			{
				textureStr += L".tga";
				auto texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);

				if (!texture)
				{
					Utils::ChangeExt(textureStr, L".png");
					texture = RESOURCES.GetOrAddTexture(textureStr, texturePath + L"\\" + textureStr);

				}
				material->Set_TextureMap(texture, TextureMapType::NORMAL);
			}
		}

		// Ambient
		material->Get_MaterialDesc().ambient = file->Read<Color>();

		// Diffuse
		material->Get_MaterialDesc().diffuse = file->Read<Color>();

		// Specular
		material->Get_MaterialDesc().specular = file->Read<Color>();



		// Emissive
		material->Get_MaterialDesc().emissive = file->Read<Color>();



		m_Materials.push_back(material);


		Bind_CacheInfo();
	}
}

void Model::ReadModel(const wstring& strPath)
{
	m_ModelTag = fs::path(strPath).filename().wstring();
	wstring fullPath = strPath + L".Model";

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	// Bones
	{
		const _uint boneCount = file->Read<_uint>();
		for (_uint i = 0; i < boneCount; ++i)
		{
			shared_ptr<ModelBone> bone = make_shared<ModelBone>();
			bone->index = file->Read<_int>();
			bone->name = Utils::ToWString(file->Read<string>());
			bone->parentIndex = file->Read<_int>();
			bone->transform = file->Read<_float4x4>();
			bone->offsetTransform = file->Read<_float4x4>();
			m_Bones.push_back(bone);


		}
	}
	// Mesh
	{
		const _uint meshCount = file->Read<_uint>();
		for (_uint i = 0; i < meshCount; ++i)
		{
			shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();

			mesh->name = Utils::ToWString(file->Read<string>());
			mesh->boneIndex = file->Read<_int>();
			mesh->materialName = Utils::ToWString(file->Read<string>());
			
			// VertexData
			{
			const _uint verticesCount = file->Read<_uint>();
				vector<ModelVertexType> vertices;
				vertices.resize(verticesCount);

				void* data = vertices.data();

				file->Read(&data, sizeof(ModelVertexType) * verticesCount);
				mesh->geometry->Add_Vertices(vertices);
			}

			//IndexData
			{
				const _uint indicesCount = file->Read<_uint>();

				vector<_uint> indices;
				indices.resize(indicesCount);

				void* data = indices.data();

				file->Read(&data, sizeof(_uint) * indicesCount);
				mesh->geometry->Add_Indices(indices);
			}

			mesh->Create_Buffer();

			m_Meshes.push_back(mesh);

		}
	}
	Bind_CacheInfo();
}

void Model::ReadAnimation(const wstring& strPath)
{
	m_AnimationTag = fs::path(strPath).filename().wstring();
	wstring fullPath = strPath + L".clip";

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	const _uint iAnimCount = file->Read<_uint>();

	for (_uint i = 0; i < iAnimCount; ++i)
	{
		shared_ptr<ModelAnimation> animation = make_shared<ModelAnimation>();

		animation->index = file->Read<_uint>();
		animation->name = Utils::ToWString(file->Read<string>());
		animation->duration = file->Read<_float>();
		animation->frameRate = file->Read<_float>();
		animation->frameCount = file->Read<_uint>();

		const _uint keyframeCount = file->Read<_uint>();

		for (_uint i = 0; i < keyframeCount; ++i)
		{
			shared_ptr<ModelKeyFrame> keyframe = make_shared<ModelKeyFrame>();
			keyframe->boneName = Utils::ToWString(file->Read<string>());
			const _uint transformSize = file->Read<_uint>();
			if (transformSize > 0)
			{
				keyframe->transforms.resize(transformSize);
				void* ptr = &keyframe->transforms[0];
				file->Read(&ptr, sizeof(ModelKeyFrameData) * transformSize);
			}
			animation->keyFrames.push_back(keyframe);
		}

		m_Animations.push_back(animation);
	}

	if (!m_pTexture)
		Create_Texture();
}

//void Model::ReadBase()
//{
//	wstring fullPath = m_strModelPath + L"Custom/Base.Base";
//	m_ModelTag = L"Base";
//	shared_ptr<FileUtils> file = make_shared<FileUtils>();
//	file->Open(fullPath, FileMode::Read);
//
//	// Bones
//	{
//		const _uint boneCount = file->Read<_uint>();
//		for (_uint i = 0; i < boneCount; ++i)
//		{
//			shared_ptr<ModelBone> bone = make_shared<ModelBone>();
//			bone->index = file->Read<_int>();
//			bone->name = Utils::ToWString(file->Read<string>());
//			bone->parentIndex = file->Read<_int>();
//			bone->transform = file->Read<_float4x4>();
//			bone->offsetTransform = file->Read<_float4x4>();
//			m_Bones.push_back(bone);
//		}
//	}
//	Bind_CacheInfo();
//}
//
//void Model::AddParts(const wstring& partName, PARTS_INFO partType)
//{
//	shared_ptr<Parts> part = RESOURCES.Get_Part(partName);
//	if (!part)
//	{
//		assert(false);
//		MSG_BOX("WrongParts");
//	}
//
//	if ((_uint)partType < 0 || (_uint)partType >= PARTS_MAX_COUNT)
//	{
//		assert(false);
//		MSG_BOX("WrongParts");
//		
//	}
//
//	switch (partType)
//	{
//	case PARTS_INFO::Hair:
//		break;
//	case PARTS_INFO::Face:
//		break;
//	case PARTS_INFO::BodyUpper:
//	{
//		if (m_Parts[(_uint)PARTS_INFO::OnePiece])
//		{
//			m_Parts[(_uint)PARTS_INFO::OnePiece] = nullptr;
//			m_Parts[(_uint)PARTS_INFO::BodyLower] = RESOURCES.Get_Part(L"Parts\\BodyLower\\LeafPants0");
//		}
//	}
//		break;
//	case PARTS_INFO::BodyLower:
//	{
//		if (m_Parts[(_uint)PARTS_INFO::OnePiece])
//		{
//			m_Parts[(_uint)PARTS_INFO::OnePiece] = nullptr;
//			m_Parts[(_uint)PARTS_INFO::BodyUpper] = RESOURCES.Get_Part(L"Parts\\BodyUpper\\LeafVest0");
//		}
//	}
//		break;
//	case PARTS_INFO::OnePiece:
//	{
//		if (m_Parts[(_uint)PARTS_INFO::BodyLower] || m_Parts[(_uint)PARTS_INFO::BodyUpper])
//		{
//			m_Parts[(_uint)PARTS_INFO::BodyLower] = nullptr;
//			m_Parts[(_uint)PARTS_INFO::BodyUpper] = nullptr;
//		}
//	}
//		break;
//	case PARTS_INFO::NonParts:
//		break;
//	case PARTS_INFO::END:
//		break;
//	default:
//		break;
//	}
//	m_Parts[(_uint)partType] = part;
//
//	Bind_CacheInfo();
//}

shared_ptr<ResourceBase> Model::Clone()
{
	shared_ptr<Model> model = make_shared<Model>();
	model->m_ModelTag = m_ModelTag;
	model->m_MaterialTag = m_MaterialTag;
	model->m_AnimationTag = m_AnimationTag;
	model->m_AnimTransforms = m_AnimTransforms;
	model->m_pTexture = m_pTexture;
	model->m_pSRV = m_pSRV;
	model->m_BoneIndexDesc = m_BoneIndexDesc;
	
	model->m_pRootBone = m_pRootBone;
	model->m_Bones = m_Bones;
	model->m_Animations = m_Animations;

	model->m_Meshes = m_Meshes;

	//for (_uint i = 0; i < PARTS_MAX_COUNT; ++i)
	//{
	//	if(m_Parts[i])
	//	model->m_Parts[i] = m_Parts[i]->Clone();
	//}

	for (auto& material : m_Materials)
	{
		model->m_Materials.push_back(static_pointer_cast<Material>(material->Clone()));
	}
	
	model->Bind_CacheInfo();
	

	return model;
}

//void Model::AddParts(const wstring& fileName)
//{
//	wstring finalPath = m_strModelPath + fileName + L".Parts";
//	ReadMaterial(fileName);
//
//	shared_ptr<FileUtils> file = make_shared<FileUtils>();
//	file->Open(finalPath, FileMode::Read);
//	PARTS_INFO partsType = PARTS_INFO(file->Read<_uint>());
//	{
//		const _uint meshCount = file->Read<_uint>();
//		for (_uint i = 0; i < meshCount; ++i)
//		{
//			shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();
//
//			mesh->name = Utils::ToWString(file->Read<string>());
//			mesh->boneIndex = file->Read<_int>();
//			mesh->materialName = Utils::ToWString(file->Read<string>());
//
//			// VertexData
//			{
//				const _uint verticesCount = file->Read<_uint>();
//				vector<ModelVertexType> vertices;
//				vertices.resize(verticesCount);
//
//				void* data = vertices.data();
//
//				file->Read(&data, sizeof(ModelVertexType) * verticesCount);
//				mesh->geometry->Add_Vertices(vertices);
//			}
//
//			//IndexData
//			{
//				const _uint indicesCount = file->Read<_uint>();
//
//				vector<_uint> indices;
//				indices.resize(indicesCount);
//
//				void* data = indices.data();
//
//				file->Read(&data, sizeof(_uint) * indicesCount);
//				mesh->geometry->Add_Indices(indices);
//			}
//
//			mesh->Create_Buffer();
//
//
//			
//
//	
//			m_Parts[(_uint)partsType].m_Meshes.push_back(mesh);
//		}
//	}
//	Bind_CacheInfo();
//}

void Model::Bind_CacheInfo()
{
	vector<shared_ptr<ModelMesh>> meshes = Get_Meshes();
	
	for (const auto& mesh : meshes)
	{
		if(!mesh->material.expired())
			continue;

		mesh->material = Get_MaterialByName(mesh->materialName);
	}

	for (const auto& mesh : meshes)
	{
		if(!mesh->bone.expired())
			continue;

		mesh->bone = Get_BoneByIndex(mesh->boneIndex);
	}

	if (m_pRootBone == nullptr && m_Bones.size() > 0)
	{
		m_pRootBone = m_Bones[0];

		for (const auto& bone : m_Bones)
		{
			if (bone->parentIndex >= 0)
			{
				bone->parent = m_Bones[max(bone->parentIndex,0)];
				bone->parent.lock()->children.push_back(bone);

			}
		}
	}
}

shared_ptr<Material> Model::Get_MaterialByName(const wstring& materialName)
{
	for (auto& material : m_Materials)
	{
		if (material->Get_Name() == materialName)
			return material;
	}
	return nullptr;
}

_uint Model::Get_MeshCount()
{
	/*_uint iMeshCount = 0;
	for (auto& part : m_Parts)
	{
		if (!part)
			continue;
		iMeshCount += _uint(part->m_Meshes.size());
	}

	return iMeshCount;*/
	return static_cast<_uint>(m_Meshes.size());
}

//vector<shared_ptr<ModelMesh>> Model::Get_Meshes()
//{
//	vector<shared_ptr<ModelMesh>> meshes;
//	for (auto& part : m_Parts)
//	{
//		if (!part)
//			continue;
//
//		meshes.insert(meshes.end(), part->m_Meshes.begin(), part->m_Meshes.end());
//
//	}
//	return meshes;
//}

shared_ptr<ModelMesh> Model::Get_MeshByName(const wstring& meshName)
{
	/*for (auto& part : m_Parts)
	{
		if (!part)
			continue;

		auto mesh = part->Get_MeshByName(meshName);

		if (mesh)
			return mesh;
	}
	return nullptr;*/

	for (auto& mesh : m_Meshes)
	{
		if (mesh->name == meshName)
			return mesh;
	}
	return nullptr;
}

shared_ptr<ModelBone> Model::Get_BoneByName(const wstring& boneName)
{
	for (auto& bone : m_Bones)
	{
		if (bone->name == boneName)
			return bone;
	}
	return nullptr;
}

_int Model::Get_BoneIndexByName(const wstring& boneName)
{
	for (_int i = 0; i < (_int)m_Bones.size(); ++i)
	{
		if (m_Bones[i]->name == boneName)
			return i;
	}

	return -1;
}

shared_ptr<ModelAnimation> Model::Get_AnimationByName(const wstring& animName, _int* pIndex)
{
	wstring name = animName;
	Utils::ToUpperString(name);
	for (_int i = 0; i < _int(m_Animations.size()); ++i)
	{
		if (m_Animations[i]->name == name)
		{
			if (pIndex)
				*pIndex = i;

			return m_Animations[i];
		}
	}
	if (pIndex)
		*pIndex = -1;
	return nullptr;
}

void Model::Create_Texture()
{

	if (Get_AnimationCount() == 0)
		return;

	m_AnimTransforms.resize(Get_AnimationCount());
	m_RootBonePosition.resize(Get_AnimationCount());
	Create_BoneData();

	for (_uint i = 0; i < Get_AnimationCount(); ++i)
		Create_AnimationTransform(i);


	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = Get_AnimationCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		const _uint dataSize = MAX_MODEL_TRANSFORMS * sizeof(_float4x4);
		const _uint pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		void* mallocPtr = ::malloc(pageSize * Get_AnimationCount());

		for (_uint c = 0; c < Get_AnimationCount(); ++c)
		{
			_uint startOffset = c * pageSize;
			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (_uint f = 0; f < MAX_MODEL_KEYFRAMES; ++f)
			{
				void* ptr = pageStartPtr + dataSize * f;
				::memcpy(ptr, m_AnimTransforms[c].transforms[f].data(), dataSize);
			}
		}

		vector<D3D11_SUBRESOURCE_DATA> subResources(Get_AnimationCount());

		for (_uint c = 0; c < Get_AnimationCount(); ++c)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = DEVICE->CreateTexture2D(&desc, subResources.data(), m_pTexture.GetAddressOf());


		::free(mallocPtr);
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof desc);
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = Get_AnimationCount();

		HRESULT hr = DEVICE->CreateShaderResourceView(m_pTexture.Get(), &desc, m_pSRV.GetAddressOf());

	}


}

void Model::Create_BoneData()
{
	for (_int i = 0; i < _int(m_Bones.size()); ++i)
	{
		m_BoneIndexDesc.transform[i] = m_Bones[i]->offsetTransform;
	}
}

void Model::Create_AnimationTransform(_uint index)
{
	vector<_float4x4> tmpAnimBoneTransforms(MAX_MODEL_TRANSFORMS, _float4x4::Identity);

	shared_ptr<ModelAnimation> animation = Get_AnimationByIndex(index);
	m_RootBonePosition[index].resize(animation->frameCount);
	for (_uint frame = 0; frame < animation->frameCount; ++frame)
	{
		for (_uint boneIndex = 0; boneIndex < Get_BoneCount(); ++boneIndex)
		{
			shared_ptr<ModelBone> bone = Get_BoneByIndex(boneIndex);

			
			_float4x4 matAnimation;

			shared_ptr<ModelKeyFrame> keyFrame = animation->Get_KeyFrame(bone->name);
			if (keyFrame)
			{
				ModelKeyFrameData& data = keyFrame->transforms[frame];

				_float4x4 S, R, T;
				S = _float4x4::CreateScale(data.scale);
				R = _float4x4::CreateFromQuaternion(data.rotation);
				if (bone->name == L"Dummy_CP")
				{
					
					m_RootBonePosition[index][frame] = _float3::Transform(data.translation, _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY( XM_PI * -0.5f)) ;
					T = _float4x4::Identity;
				}
				else
					T = _float4x4::CreateTranslation(data.translation);
				
				matAnimation = S * R * T;
				//DirectX::XMStoreFloat4x4(&matAnimation, XMMatrixAffineTransformation(XMLoadFloat3(&(data.scale)), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&data.rotation), XMLoadFloat3(&data.translation)));
			}
			else
			{
 				matAnimation = _float4x4::Identity;
			}

			_int parentIndex = bone->parentIndex;

			_float4x4 matParent = _float4x4::Identity;
			if (parentIndex >= 0)
				matParent = tmpAnimBoneTransforms[parentIndex];

			tmpAnimBoneTransforms[boneIndex] = matAnimation * matParent;		
			m_AnimTransforms[index].transforms[frame][boneIndex] = tmpAnimBoneTransforms[boneIndex];
		}
	}
}

