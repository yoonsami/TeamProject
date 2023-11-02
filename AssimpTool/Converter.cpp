#include "pch.h"
#include "Converter.h"

Converter::Converter()
{
	m_pImporter = make_shared<Assimp::Importer>();
}

Converter::~Converter()
{
	
}

void Converter::ReadAssetFile(const wstring& file)
{
	
	wstring fileStr = file;

	auto p = fs::path(fileStr);
	assert(fs::exists(p));
	
	m_pScene = m_pImporter->ReadFile(
		Utils::ToString(fileStr),
		aiProcess_ConvertToLeftHanded |
		aiProcessPreset_TargetRealtime_Fast
	);
	 
	assert(m_pScene);
}

void Converter::ExportModelData(const wstring& savePath)
{
	fs::create_directories(fs::path(m_strModelPath + savePath));
	wstring finalPath = m_strModelPath + savePath + L"\\" + savePath + L".Model";
	ReadModelData(m_pScene->mRootNode, -1, -1);
	ReadSkinData();

	WriteCSVFile();



	WriteModelFile(finalPath);
}

void Converter::ExportMaterialData(const wstring& savePath)
{
	fs::create_directories(fs::path(m_strModelPath + savePath));
	wstring finalPath = m_strModelPath + savePath + L"\\" + savePath + L".Material";
	ReadMaterialData();
	WriteMaterialData(finalPath);
	WriteTexture(savePath);
}

void Converter::ExportAnimationData(const wstring& savePath)
{
	const _uint count = m_pScene->mNumAnimations;

	for (_uint i = 0; i < count; ++i)
	{
		shared_ptr<asAnimation> animation = ReadAnimationData(m_pScene->mAnimations[i]);
		
		if (animation->name == "Take 001")
			continue;

		m_Animations.push_back(animation);
	}

	wstring finalPath = m_strModelPath + savePath + L"\\" + savePath + L".clip";
	WriteAnimationData(finalPath);

}

//void Converter::ReadPartsAnimationData(const wstring& savePath)
//{
//	const _uint count = m_pScene->mNumAnimations;
//
//	for (_uint i = 0; i < count; ++i)
//	{
//		shared_ptr<asAnimation> animation = ReadAnimationData(m_pScene->mAnimations[i]);
//
//		if (animation->name == "Take 001")
//			continue;
//
//		m_Animations.push_back(animation);
//	}
//}
//
//void Converter::ExportPartsAnimationData()
//{
//	wstring animPath = m_strModelPath + L"Custom/Base.clip";
//	WriteAnimationData(animPath);
//}
//
//void Converter::ExportBaseData(const wstring& savePath)
//{
//	wstring finalPath = m_strModelPath + savePath + L".Base";
//	ReadBaseData(m_pScene->mRootNode, -1, -1);
//
//
//	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
//	{
//		aiMesh* srcMesh = m_pScene->mMeshes[i];
//		if (!srcMesh->HasBones())
//			continue;
//
//		shared_ptr<asMesh> mesh = m_Mesh[i];
//
//		// Bone 순회 -> 연관된 vertexindex, weight 구함
//		for (_uint b = 0; b < srcMesh->mNumBones; ++b)
//		{
//			aiBone* srcMeshBone = srcMesh->mBones[b];
//			_uint boneIndex = Get_BoneIndex(srcMeshBone->mName.C_Str());
//
//			_float4x4 offsetMatrix = _float4x4(srcMeshBone->mOffsetMatrix[0]);
//			m_Bone[boneIndex]->offsetTransform = offsetMatrix.Transpose();
//
//		}
//	}
//
//	WriteBaseFile(finalPath);
//}
//
//void Converter::ExportPartsData(const wstring& savePath)
//{
//	wstring finalPath = m_strModelPath + savePath + L".Parts";
//
//	m_Mesh.clear();
//	m_Material.clear();
//	ReadPartsModelData();
//	ReadPartsSkinData();
//	ExportMaterialData(savePath);
//
//	WritePartsFile(finalPath);
//}

// aiNode : 상속관계 표현
//
void Converter::ReadModelData(aiNode* node, _int index, _int parent)
{
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();
	//Relative transform
	_float4x4 transform(node->mTransformation[0]);
	bone->transform = transform.Transpose();

	// m_bone의 transform 은 root transform(root부터 타고 들어가기때문)
	_float4x4 matParent = _float4x4::Identity;
	if (parent >= 0)
		matParent = m_Bone[parent]->transform;

	// local transform
	bone->transform = bone->transform * matParent;

	m_Bone.push_back(bone);

	//Mesh
	ReadMeshData(node, index);

	for (_uint i = 0; i < node->mNumChildren; ++i)
	{
		ReadModelData(node->mChildren[i], _int(m_Bone.size()), index);
	}

}

void Converter::ReadBaseData(aiNode* node, _int index, _int parent)
{
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	//Relative transform
	_float4x4 transform(node->mTransformation[0]);
	bone->transform = transform.Transpose();

	// m_bone의 transform 은 root transform(root부터 타고 들어가기때문)
	_float4x4 matParent = _float4x4::Identity;
	if (parent >= 0)
		matParent = m_Bone[parent]->transform;

	// local transform
	bone->transform = bone->transform * matParent;

	m_Bone.push_back(bone);

	for (_uint i = 0; i < node->mNumChildren; ++i)
	{
		ReadModelData(node->mChildren[i], _int(m_Bone.size()), index);
	}

}

void Converter::ReadMeshData(aiNode* node, _int bone)
{
	if (node->mNumMeshes < 1)
		return;


	for (_uint i = 0; i < node->mNumMeshes; ++i)
	{
		shared_ptr<asMesh> mesh = make_shared<asMesh>();
		mesh->name = node->mName.C_Str();
		mesh->boneIndex = bone;
		_uint index = node->mMeshes[i];
		const aiMesh* srcMesh = m_pScene->mMeshes[index];

		const aiMaterial* material = m_pScene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		for (_uint v = 0; v < srcMesh->mNumVertices; ++v)
		{
			VertexType vertex;
			memcpy(&vertex.vPosition, &(srcMesh->mVertices[v]), sizeof(_float3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.vTexCoord, &srcMesh->mTextureCoords[0][v], sizeof(_float2));

			if (srcMesh->HasNormals())
			{
				memcpy(&vertex.vNormal, &srcMesh->mNormals[v], sizeof(_float3));
			}

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.vTangent, &srcMesh->mTangents[v], sizeof(_float3));

			vertex.iFiltered = 0;
		
			mesh->vertices.push_back(vertex);
		}

		for (_uint f = 0; f < srcMesh->mNumFaces; ++f)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (_uint k = 0; k < face.mNumIndices; ++k)
				mesh->indices.push_back(face.mIndices[k]);
		}
		m_Mesh.push_back(mesh);
	}

}

void Converter::ReadSkinData()
{
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		aiMesh* srcMesh = m_pScene->mMeshes[i];
		if(!srcMesh->HasBones())
			continue;

		shared_ptr<asMesh> mesh = m_Mesh[i];

		vector<asBoneWeight> VertexBoneWeights;
		VertexBoneWeights.resize(mesh->vertices.size());

		// Bone 순회 -> 연관된 vertexindex, weight 구함
		for (_uint b = 0; b < srcMesh->mNumBones; ++b)
		{
			aiBone* srcMeshBone = srcMesh->mBones[b];
			_uint boneIndex = Get_BoneIndex(srcMeshBone->mName.C_Str());

			_float4x4 offsetMatrix = _float4x4(srcMeshBone->mOffsetMatrix[0]);
			m_Bone[boneIndex]->offsetTransform = offsetMatrix.Transpose();

			for (_uint w = 0; w < srcMeshBone->mNumWeights; ++w)
			{
				_uint index =srcMeshBone->mWeights[w].mVertexId;
				_float weight = srcMeshBone->mWeights[w].mWeight;

				VertexBoneWeights[index].Add_Weights(boneIndex, weight);
			}
		}

		for (_uint v = 0; v < _uint(VertexBoneWeights.size()); ++v)
		{
			VertexBoneWeights[v].Normalize();

			asBlendWeight blendWeight = VertexBoneWeights[v].Get_BlendWeight();
			mesh->vertices[v].vBlendIndices = blendWeight.indices;
			mesh->vertices[v].vBlendWeights = blendWeight.weights;
		}

		if (srcMesh->mNumBones == 0)
		{
			mesh->boneIndex = Get_BoneIndex(mesh->name);


		}
	}
}

void Converter::WriteCSVFile()
{
	FILE* file;
	::fopen_s(&file, "../Vertices.csv", "w");

	assert(file != nullptr);

	for (auto& bone : m_Bone)
	{
		string name = bone->name;
		::fprintf(file, "%d,%s\n", bone->index, bone->name.c_str());
	}

	::fprintf(file, "\n");
	for (auto& mesh : m_Mesh)
	{
		string name = mesh->name;
		::printf("%s\n", name.c_str());

		for (_uint i = 0; i < _uint(mesh->vertices.size()); ++i)
		{
			_float3 p = mesh->vertices[i].vPosition;
			_float4 indices = mesh->vertices[i].vBlendIndices;
			_float4 weights = mesh->vertices[i].vBlendWeights;

			::fprintf(file, "%f, %f, %f,", p.x, p.y, p.z);
			::fprintf(file, "%f, %f, %f, %f, ", indices.x, indices.y, indices.z, indices.w);
			::fprintf(file, "%f, %f, %f, %f\n ", weights.x, weights.y, weights.z, weights.w);

		}
	}

	::fclose(file);

}

void Converter::WriteModelFile(const wstring& finalPath)
{
	auto path = fs::path(finalPath);
	fs::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	file->Write<_uint>(_uint(m_Bone.size()));
	for (auto& bone : m_Bone)
	{
		file->Write<_int>(bone->index);
		file->Write<string>(bone->name);
		file->Write<_int>(bone->parent);
		file->Write<_float4x4>(bone->transform);
		file->Write<_float4x4>(bone->offsetTransform);
	}

	file->Write<_uint>(_uint(m_Mesh.size()));
	for (auto& mesh : m_Mesh)
	{
		file->Write<string>(mesh->name);
		file->Write<_int>(mesh->boneIndex);
		file->Write<string>(mesh->materialName);

		file->Write<_uint>(_uint(mesh->vertices.size()));
		file->Write(&mesh->vertices[0], _uint((sizeof(VertexType) * mesh->vertices.size())));
		
		file->Write<_uint>(_uint(mesh->indices.size()));
		file->Write(&mesh->indices[0], _uint(sizeof(_uint) * mesh->indices.size()));
	}
}

//void Converter::WriteBaseFile(const wstring& finalPath)
//{
//	auto path = fs::path(finalPath);
//	fs::create_directory(path.parent_path());
//
//	shared_ptr<FileUtils> file = make_shared<FileUtils>();
//	file->Open(finalPath, FileMode::Write);
//
//	file->Write<_uint>(_uint(m_Bone.size()));
//	for (auto& bone : m_Bone)
//	{
//		file->Write<_int>(bone->index);
//		file->Write<string>(bone->name);
//		file->Write<_int>(bone->parent);
//		file->Write<_float4x4>(bone->transform);
//		file->Write<_float4x4>(bone->offsetTransform);
//	}
//}

//void Converter::WritePartsFile(const wstring& finalPath)
//{
//	auto path = fs::path(finalPath);
//	fs::create_directory(path.parent_path());
//
//	shared_ptr<FileUtils> file = make_shared<FileUtils>();
//	file->Open(finalPath, FileMode::Write);
//
//	_uint partsType = 0;
//	if (finalPath.find(L"BodyUpper") != wstring::npos)
//		partsType = _uint(PARTS_INFO::BodyUpper);
//
//	else if (finalPath.find(L"BodyLower") != wstring::npos)
//		partsType = _uint(PARTS_INFO::BodyLower);
//
//	else if (finalPath.find(L"Face") != wstring::npos)
//		partsType = _uint(PARTS_INFO::Face);
//
//	else if (finalPath.find(L"HeadGear") != wstring::npos)
//		partsType = _uint(PARTS_INFO::Hair);
//
//	else if (finalPath.find(L"OnePiece") != wstring::npos)
//		partsType = _uint(PARTS_INFO::OnePiece);
//
//	else
//		partsType = _uint(PARTS_INFO::NonParts);
//
//	file->Write<_uint>(partsType);
//
//	file->Write<_uint>(_uint(m_Mesh.size()));
//	for (auto& mesh : m_Mesh)
//	{
//		file->Write<string>(mesh->name);
//		file->Write<_int>(mesh->boneIndex);
//		file->Write<string>(mesh->materialName);
//
//		file->Write<_uint>(_uint(mesh->vertices.size()));
//		file->Write(&mesh->vertices[0], _uint((sizeof(VertexType) * mesh->vertices.size())));
//
//		file->Write<_uint>(_uint(mesh->indices.size()));
//		file->Write(&mesh->indices[0], _uint(sizeof(_uint) * mesh->indices.size()));
//		
//	}
//
//}
//
//void Converter::ReadPartsModelData()
//{
//	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
//	{
//		shared_ptr<asMesh> mesh = make_shared<asMesh>();
//		mesh->name = m_pScene->mMeshes[i]->mName.C_Str();
//		mesh->boneIndex = 0;
//		const aiMesh* srcMesh = m_pScene->mMeshes[i];
//
//		const aiMaterial* material = m_pScene->mMaterials[srcMesh->mMaterialIndex];
//		mesh->materialName = material->GetName().C_Str();
//
//		for (_uint v = 0; v < srcMesh->mNumVertices; ++v)
//		{
//			VertexType vertex;
//			memcpy(&vertex.vPosition, &(srcMesh->mVertices[v]), sizeof(_float3));
//
//			if (srcMesh->HasTextureCoords(0))
//				memcpy(&vertex.vTexCoord, &srcMesh->mTextureCoords[0][v], sizeof(_float2));
//
//			if (srcMesh->HasNormals())
//			{
//				memcpy(&vertex.vNormal, &srcMesh->mNormals[v], sizeof(_float3));
//			}
//
//			if (srcMesh->HasTangentsAndBitangents())
//				memcpy(&vertex.vTangent, &srcMesh->mTangents[v], sizeof(_float3));
//
//
//			mesh->vertices.push_back(vertex);
//		}
//
//		for (_uint f = 0; f < srcMesh->mNumFaces; ++f)
//		{
//			aiFace& face = srcMesh->mFaces[f];
//
//			for (_uint k = 0; k < face.mNumIndices; ++k)
//				mesh->indices.push_back(face.mIndices[k]);
//		}
//		m_Mesh.push_back(mesh);
//	}
//}
//
//void Converter::ReadPartsSkinData()
//{
//	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
//	{
//		aiMesh* srcMesh = m_pScene->mMeshes[i];
//		if (!srcMesh->HasBones())
//			continue;
//
//		shared_ptr<asMesh> mesh = m_Mesh[i];
//
//		vector<asBoneWeight> VertexBoneWeights;
//		VertexBoneWeights.resize(mesh->vertices.size());
//
//		// Bone 순회 -> 연관된 vertexindex, weight 구함
//		for (_uint b = 0; b < srcMesh->mNumBones; ++b)
//		{
//			aiBone* srcMeshBone = srcMesh->mBones[b];
//			_uint boneIndex = Get_BoneIndex(srcMeshBone->mName.C_Str());
//
//			_float4x4 offsetMatrix = _float4x4(srcMeshBone->mOffsetMatrix[0]);
//			m_Bone[boneIndex]->offsetTransform = offsetMatrix.Transpose();
//
//			for (_uint w = 0; w < srcMeshBone->mNumWeights; ++w)
//			{
//				_uint index = srcMeshBone->mWeights[w].mVertexId;
//				_float weight = srcMeshBone->mWeights[w].mWeight;
//
//				VertexBoneWeights[index].Add_Weights(boneIndex, weight);
//			}
//		}
//
//		for (_uint v = 0; v < _uint(VertexBoneWeights.size()); ++v)
//		{
//			VertexBoneWeights[v].Normalize();
//
//			asBlendWeight blendWeight = VertexBoneWeights[v].Get_BlendWeight();
//			mesh->vertices[v].vBlendIndices = blendWeight.indices;
//			mesh->vertices[v].vBlendWeights = blendWeight.weights;
//		}
//
//		if (srcMesh->mNumBones == 0)
//		{
//			mesh->boneIndex = Get_BoneIndex(mesh->name);
//
//
//		}
//	}
//}


void Converter::ReadMaterialData()
{
	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		aiMaterial* srcMaterial = m_pScene->mMaterials[i];

		shared_ptr<asMaterial> material = make_shared<asMaterial>();
		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Color(color.r, color.g, color.b, 1.f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Color(color.r, color.g, color.b, 1.f);
		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->emissive = Color(color.r, color.g, color.b, 1.f);

		{
			aiString file;

			srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			material->diffuseFile = file.C_Str();
			Utils::DetachExt(material->diffuseFile);
			fs::path ps = fs::path(material->diffuseFile);
			material->diffuseFile = ps.filename().string();
		}
		{
			aiString file;
			srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
			material->normalFile = file.C_Str();
			Utils::DetachExt(material->normalFile);
			fs::path ps = fs::path(material->normalFile);
			material->normalFile = ps.filename().string();
		}
		{
			aiString file;
			srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
			material->specularFile = file.C_Str();
			Utils::DetachExt(material->specularFile);
			fs::path ps = fs::path(material->specularFile);
			material->specularFile = ps.filename().string();
		}

		m_Material.push_back(material);
	}
}

void Converter::WriteMaterialData(const wstring& finalPath)
{
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	file->Write<_uint>(static_cast<_uint>(m_Material.size()));
	for (shared_ptr<asMaterial> material : m_Material)
	{
		file->Write<string>(material->name);

		_bool flag = !material->diffuseFile.empty();
		file->Write<_bool>(flag);
		if (flag)
		{
			file->Write<string>(material->diffuseFile);
		}

		flag = !material->specularFile.empty();
		file->Write<_bool>(flag);
		if (flag)
		{
			file->Write<string>(material->specularFile);
		}

		flag = !material->normalFile.empty();
		file->Write<_bool>(flag);
		if (flag)
		{
			file->Write<string>(material->normalFile);
		}
		
		file->Write<Color>(material->ambient);
		file->Write<Color>(material->diffuse);
		file->Write<Color>(material->specular);
		file->Write<Color>(material->emissive);
	}
}

void Converter::WriteTexture(const wstring& assetName)
{
	wstring assetTexturePath = m_strAssetPath + assetName + L"\\";

	for (auto& entry : fs::recursive_directory_iterator(assetTexturePath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".DDS" 
			&& entry.path().extension().wstring() != L".dds" 
			&& entry.path().extension().wstring() != L".png"
			&& entry.path().extension().wstring() != L".PNG"
			&& entry.path().extension().wstring() != L".tga"
			&& entry.path().extension().wstring() != L".TGA")
			continue;

		fs::create_directories(fs::path(m_strTexturePath + assetName));
		string fileName = entry.path().filename().string();
		string targetPath = (fs::path(m_strTexturePath + assetName) / fileName).string();
		Utils::Replace(targetPath, "\\", "/");
		::CopyFileA(entry.path().string().c_str(), targetPath.c_str(), false);

	}
}

shared_ptr<asAnimation> Converter::ReadAnimationData(aiAnimation* srcAnimation)
{
	shared_ptr<asAnimation> animation = make_shared<asAnimation>();

	animation->name = srcAnimation->mName.C_Str();

	if (animation->name == "Take 001")
		return animation;

	animation->frameRate = _float(srcAnimation->mTicksPerSecond);
	animation->frameCount = _uint(srcAnimation->mDuration + 1);

	map<string, shared_ptr<asAnimationNode>> cacheAnimNodes;

	for (_uint i = 0; i < srcAnimation->mNumChannels; ++i)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		shared_ptr<asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		animation->duration = max(animation->duration, node->keyFrame.back().time);

		cacheAnimNodes[srcNode->mNodeName.C_Str()] = node;
	}

	ReadKeyFrameData(animation, m_pScene->mRootNode, cacheAnimNodes);

	return animation;
}

shared_ptr<asAnimationNode> Converter::ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode)
{
	shared_ptr<asAnimationNode> node = make_shared<asAnimationNode>();

	node->name = srcNode->mNodeName;
	if (node->name == aiString("root"))
	{
		int a = 0;
	}
	_uint keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumRotationKeys), srcNode->mNumScalingKeys);
	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for (_uint k = 0; k < keyCount; ++k)
	{
		asKeyFrameData frameData;


		/*_bool found = false;
		_uint t = _uint(node->keyFrame.size());*/

		if (srcNode->mNumScalingKeys > k)
		{
			memcpy(&vScale, &srcNode->mScalingKeys[k].mValue, sizeof _float3);
			frameData.time = srcNode->mScalingKeys[k].mTime;
		}

		if (srcNode->mNumRotationKeys > k)
		{
			vRotation.x = srcNode->mRotationKeys[k].mValue.x;
			vRotation.y = srcNode->mRotationKeys[k].mValue.y;
			vRotation.z = srcNode->mRotationKeys[k].mValue.z;
			vRotation.w = srcNode->mRotationKeys[k].mValue.w;
			frameData.time = srcNode->mRotationKeys[k].mTime;
		}
		if (srcNode->mNumPositionKeys > k)
		{
			memcpy(&vPosition, &srcNode->mPositionKeys[k].mValue, sizeof _float3);
			frameData.time = srcNode->mPositionKeys[k].mTime;
		}

		frameData.scale = vScale;
		frameData.rotation = vRotation;
		frameData.translation = vPosition;

		//CHANNEL
		node->keyFrame.push_back(frameData);
		////position
		
	}

	if (node->keyFrame.size() < animation->frameCount)
	{
		_uint count = animation->frameCount - node->keyFrame.size();
		asKeyFrameData keyFrame = node->keyFrame.back();

		for (_uint n = 0; n < count; n++)
			node->keyFrame.push_back(keyFrame);
	}

	return node;
}

void Converter::ReadKeyFrameData(shared_ptr<asAnimation> animation, aiNode* srcNode, map<string, shared_ptr<asAnimationNode>>& cache)
{
	shared_ptr<asKeyFrame> keyframe = make_shared<asKeyFrame>();
	keyframe->boneName = srcNode->mName.C_Str();

	auto findNode = cache[srcNode->mName.C_Str()];

	for (_uint i = 0; i < animation->frameCount; ++i)
	{
		asKeyFrameData frameData;

		if (findNode == nullptr)
		{
			_float4x4 transform(srcNode->mTransformation[0]);
			transform = transform.Transpose();
			frameData.time = (_float)i;
			transform.Decompose(OUT frameData.scale, OUT frameData.rotation, OUT frameData.translation);

		}
		else
		{
			// 보간해서 채워넣기 시도
			frameData = findNode->keyFrame[i];
		}

		keyframe->transforms.push_back(frameData);

	}

	animation->keyFrames.push_back(keyframe);

	for (_uint i = 0; i < srcNode->mNumChildren; ++i)
		ReadKeyFrameData(animation, srcNode->mChildren[i], cache);
}

void Converter::WriteAnimationData(const wstring& finalPath)
{
	auto path = fs::path(finalPath);

	fs::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);


	_uint AnimationCount = _uint(m_Animations.size());
	file->Write<_uint>(AnimationCount);
	
	for (_uint i =0; i< AnimationCount;++i)
	{
		auto& animation = m_Animations[i];
		
		file->Write<_uint>(i);
		file->Write<string>(animation->name);
		file->Write<_float>(animation->duration);
		file->Write<_float>(animation->frameRate);
		file->Write<_uint>(animation->frameCount);

		file->Write<_uint>(_uint(animation->keyFrames.size()));

		for (auto& keyframe : animation->keyFrames)
		{
			file->Write<string>(keyframe->boneName);
			file->Write<_uint>(_uint(keyframe->transforms.size()));
			file->Write(&keyframe->transforms[0], _uint(sizeof(asKeyFrameData) * keyframe->transforms.size()));
		}
	}


}

_int Converter::Get_BoneIndex(const string& name)
{
	for (shared_ptr<asBone>& bone : m_Bone)
	{
		if (bone->name == name)
			return bone->index;
	}

	return -1;
}

void Converter::ApplyScaleToNode(aiNode* node, const aiMatrix4x4& scale)
{
	//node->mTransformation = scale * node->mTransformation;

	//for (_uint i = 0; i < node->mNumChildren; ++i)
		//ApplyScaleToNode(node->mChildren[i], scale);
}

void Converter::ApplyScaleToAnimations(const aiMatrix4x4& scale)
{
	if (m_pScene->HasAnimations()) {
		for (unsigned int i = 0; i < m_pScene->mNumAnimations; i++) {
			aiAnimation* animation = m_pScene->mAnimations[i];

			// 애니메이션의 모든 프레임에 스케일 적용
			for (unsigned int j = 0; j < animation->mNumChannels; j++) {
				aiNodeAnim* channel = animation->mChannels[j];

				// 위치 키 프레임에 스케일 적용
				for (unsigned int k = 0; k < channel->mNumPositionKeys; k++) {
					channel->mPositionKeys[k].mValue = scale * channel->mPositionKeys[k].mValue;
				}

				// 스케일 행렬을 스케일 키 프레임에 적용
				for (unsigned int k = 0; k < channel->mNumScalingKeys; k++) {
					channel->mScalingKeys[k].mValue = scale * channel->mScalingKeys[k].mValue;
				}
			}
		}
	}
}
