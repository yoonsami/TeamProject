#include "pch.h"
#include "ResourceMgr.h"
#include "Utils.h"
#include <filesystem>
#include "Model.h"
#include "Material.h"
#include "FileUtils.h"
#include "ModelMesh.h"
#include "Geometry.h"
#include "CustomFont.h"
#include "MathUtils.h"
#include "MeshEffectData.h"
#include "ParticleData.h"
#include "GroupEffectData.h"

namespace fs = std::filesystem;

ResourceMgr::~ResourceMgr()
{
	
}
void ResourceMgr::Initialize()
{
	if (!RESOURCES.Get<Texture>(L"SSAO_RandomVectorTexture"))
	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = 256;
		texDesc.Height = 256;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.SysMemPitch = 256 * sizeof(Color);

		vector<Color> color(256 * 256);

		for (_int i = 0; i < 256; ++i)
		{
			for (_int j = 0; j < 256; ++j)
			{
				XMFLOAT3 v(MathUtils::RandF(), MathUtils::RandF(), MathUtils::RandF());

				color[i * 256 + j] = Color(v.x, v.y, v.z, 0.0f);
			}
		}

		initData.pSysMem = color.data();

		ComPtr<ID3D11Texture2D> tex;
		(DEVICE->CreateTexture2D(&texDesc, &initData, tex.GetAddressOf()));

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->CreateFromResource(tex);

		RESOURCES.Add<Texture>(L"SSAO_RandomVectorTexture", texture);

	}

	CreateDefaultMesh();
	CreateDefaultShader();
	//CreateDefaultShader_EffectTool(); // MEMO : Effect Tool에서 사용하는 Shader Create 함수 
	CreateDefaultMaterial();
	CreateDefaultFont();

	CreateMeshEffectData();
	CreateParticleData();
	CreateGroupEffectData();
}


shared_ptr<Texture> ResourceMgr::GetOrAddTexture(const wstring& key, const wstring& path)
{
	auto texture = Get<Texture>(key);
	if (key == L"")
		return nullptr;
	if (fs::exists(fs::path(path)) == false)
		return nullptr;

	texture = Load<Texture>(key, path);

	if (texture == nullptr)
	{
		texture = make_shared<Texture>();
		texture->Load(path);

		wstring name = key;
		Utils::DetachExt(name);
		texture->Set_Name(name);
		Add(name, texture);
	}

	return texture;
}

shared_ptr<GroupEffectData> ResourceMgr::GetOrAddGroupEffectData(const wstring& key, const wstring& path)
{
	auto groupEffectData = Get<GroupEffectData>(key);
	if (key == L"")
		return nullptr;
	if (fs::exists(fs::path(path)) == false)
		return nullptr;

	groupEffectData = Load<GroupEffectData>(key, path);

	if (groupEffectData == nullptr)
	{
		groupEffectData = make_shared<GroupEffectData>();
		groupEffectData->Load(path);

		wstring name = key;
		Utils::DetachExt(name);
		groupEffectData->Set_Name(name);
		Add(name, groupEffectData);
	}

	return groupEffectData;
}

shared_ptr<GroupEffectData> ResourceMgr::ReloadGroupEffectData(const wstring& key, const wstring& path)
{
	wstring name = key;
	Utils::DetachExt(name);
	auto groupEffectData = Get<GroupEffectData>(name);
	if (key == L"" || nullptr == groupEffectData)
		return nullptr;

	// For. Create Reloaded 
	auto reloadedGroupEffectData = make_shared<GroupEffectData>();
	reloadedGroupEffectData->Load(path);
	
	reloadedGroupEffectData->Set_Name(name);

	// For. Erase prev 
	Delete<GroupEffectData>(name);

	// For. Add reloaded 
	//Add(key, reloadedGroupEffectData);
	Add(name, reloadedGroupEffectData);

	return reloadedGroupEffectData;
}

shared_ptr<MeshEffectData> ResourceMgr::ReloadOrAddMeshEffectData(const wstring& key, const wstring& path)
{
	wstring name = key;
	Utils::DetachExt(name);
	if (key == L"")
		return nullptr;

	auto meshEffectData = Get<MeshEffectData>(name);
	
	auto newMeshEffectData = make_shared<MeshEffectData>();
	newMeshEffectData->Load(path);
	newMeshEffectData->Set_Name(name);

	// For. Add New Mesh Effect Data 
	if (nullptr == meshEffectData)
		Add(name, newMeshEffectData);

	// For. Delete prev Mesh Effect Data and Add new Mesh Effect Data
	else
	{
		Delete<GroupEffectData>(name);	// For. Delete prev 
		Add(name, newMeshEffectData);	// For. Add new 
	}
	return newMeshEffectData;
}


//shared_ptr<Parts> ResourceMgr::Get_Part(const wstring& key)
//{
//	_uint type;
//	if (key.find(L"BodyUpper") != wstring::npos)
//		type = _uint(PARTS_INFO::BodyUpper);
//
//	else if (key.find(L"BodyLower") != wstring::npos)
//		type = _uint(PARTS_INFO::BodyLower);
//
//	else if (key.find(L"Face") != wstring::npos)
//		type = _uint(PARTS_INFO::Face);
//
//	else if (key.find(L"HeadGear") != wstring::npos)
//		type = _uint(PARTS_INFO::Hair);
//
//	else if (key.find(L"OnePiece") != wstring::npos)
//		type = _uint(PARTS_INFO::OnePiece);
//
//	else
//		type = _uint(PARTS_INFO::NonParts);
//
//	return m_PrototypeParts[type][key];
//}

//void ResourceMgr::LoadBase()
//{
//	shared_ptr<Model> m1 = make_shared<Model>();
//	m1->ReadBase();
//	m1->ReadAnimation(L"Custom/Base");
//
//	Add(L"Base_Model", m1);
//}

//void ResourceMgr::LoadParts()
//{
//	wstring assetPath = L"..\\Resources\\Models\\Custom\\Parts\\";
//
//	for (auto& entry : fs::recursive_directory_iterator(assetPath))
//	{
//		if (entry.is_directory())
//			continue;
//
//		if (entry.path().extension().wstring() != L".Parts" && entry.path().extension().wstring() != L".parts")
//			continue;
//
//		// ..\\Resources\\Models\\Custom\\Parts\\BodyLower\\Anbu1.Parts
//
//		wstring tag = entry.path().wstring();
//		//..\\Resources\\Models\\Custom\\Parts\\BodyLower\\Anbu1
//		Utils::DetachExt(tag);
//
//		int i = int(tag.rfind(L"Parts\\"));
//
//		//Parts\\BodyLower\\Anbu1
//		tag = tag.substr(i, tag.size() - i);
//
//		wstring materialPath = m_strTexturePath+L"Custom/" + tag + L".Material";
//		wstring modelPath = m_strModelPath + L"Custom/" + tag + L".Parts";
//
//		{
//			shared_ptr<FileUtils> file = make_shared<FileUtils>();
//
//			file->Open(materialPath, FileMode::Read);
//
//			while (true)
//			{
//				shared_ptr<Material> material = make_shared<Material>();
//
//				material->Set_Name(Utils::ToWString(file->Read<string>()));
//
//				if (file->Read<_bool>())
//				{
//					wstring textureStr = Utils::ToWString(file->Read<string>());
//					if (textureStr.length() > 0)
//					{
//						textureStr += L".dds";
//						auto texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						if (!texture)
//						{
//							Utils::ChangeExt(textureStr, L".tga");
//							texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						}
//						material->Set_DiffuseMap(texture);
//					}
//				}
//
//				// Specular Texture
//				if (file->Read<_bool>())
//				{
//					wstring textureStr = Utils::ToWString(file->Read<string>());
//					if (textureStr.length() > 0)
//					{
//						textureStr += L".dds";
//						auto texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						if (!texture)
//						{
//							Utils::ChangeExt(textureStr, L".tga");
//							texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						}
//						material->Set_SpecularMap(texture);
//					}
//				}
//
//				// Normal Texture
//				if (file->Read<_bool>())
//				{
//					wstring textureStr = Utils::ToWString(file->Read<string>());
//					if (textureStr.length() > 0)
//					{
//						textureStr += L".tga";
//						auto texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						material->Set_NormalMap(texture);
//					}
//				}
//
//				if (file->Read<_bool>())
//				{
//					wstring textureStr = Utils::ToWString(file->Read<string>());
//					if (textureStr.length() > 0)
//					{
//						textureStr += L".dds";
//						auto texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						material->Set_FilterMap(texture);
//					}
//				}
//
//				if (file->Read<_bool>())
//				{
//					wstring textureStr = Utils::ToWString(file->Read<string>());
//					if (textureStr.length() > 0)
//					{
//						textureStr += L".tga";
//						auto texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						material->Set_FilterNormalMap(texture);
//					}
//				}
//
//				if (file->Read<_bool>())
//				{
//					wstring textureStr = Utils::ToWString(file->Read<string>());
//					if (textureStr.length() > 0)
//					{
//						textureStr += L".dds";
//						auto texture = RESOURCES.GetOrAddTexture(textureStr, m_strTexturePath + textureStr);
//						material->Set_MaskMap(texture);
//					}
//				}
//
//
//
//				// Ambient
//				material->Get_MaterialDesc().ambient = file->Read<Color>();
//
//				// Diffuse
//				material->Get_MaterialDesc().diffuse = file->Read<Color>();
//
//				// Specular
//				material->Get_MaterialDesc().specular = file->Read<Color>();
//
//				// Emissive
//				material->Get_MaterialDesc().emissive = file->Read<Color>();
//
//
//				if (!Get<Material>(material->Get_Name()))
//				{
//					Add<Material>(material->Get_Name(), material);
//					Get<Model>(L"Base_Model")->Get_Materials().push_back(material);
//				}
//
//				if (!file->Read<_bool>())
//					break;
//			}
//
//		}
//		{
//			shared_ptr<Parts> parts = make_shared<Parts>();
//
//			shared_ptr<FileUtils> file = make_shared<FileUtils>();
//			file->Open(modelPath, FileMode::Read);
//
//			PARTS_INFO partsType = PARTS_INFO(file->Read<_uint>());
//			m_PrototypeParts[(_uint)partsType][tag] = parts;
//
//			{
//				const _uint meshCount = file->Read<_uint>();
//				for (_uint i = 0; i < meshCount; ++i)
//				{
//					shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();
//
//					mesh->name = Utils::ToWString(file->Read<string>());
//					mesh->boneIndex = file->Read<_int>();
//					mesh->materialName = Utils::ToWString(file->Read<string>());
//
//					// VertexData
//					{
//						const _uint verticesCount = file->Read<_uint>();
//						vector<ModelVertexType> vertices;
//						vertices.resize(verticesCount);
//
//						void* data = vertices.data();
//
//						file->Read(&data, sizeof(ModelVertexType) * verticesCount);
//						mesh->geometry->Add_Vertices(vertices);
//					}
//
//					//IndexData
//					{
//						const _uint indicesCount = file->Read<_uint>();
//
//						vector<_uint> indices;
//						indices.resize(indicesCount);
//
//						void* data = indices.data();
//
//						file->Read(&data, sizeof(_uint) * indicesCount);
//						mesh->geometry->Add_Indices(indices);
//					}
//
//					mesh->Create_Buffer();
//
//					m_PrototypeParts[(_uint)partsType][tag]->m_Meshes.push_back(mesh);
//				}
//			}
//				}
//	}
//}

void ResourceMgr::CreateDefaultMesh()
{
	{
		auto pMesh = make_shared<Mesh>();
		pMesh->Set_Name(L"Quad");
		pMesh->CreateQuad();
		Add(pMesh->Get_Name(), pMesh);
	}
	{
		auto pMesh = make_shared<Mesh>();
		pMesh->Set_Name(L"Cube");
		pMesh->CreateCube();
		Add(pMesh->Get_Name(), pMesh);
	}
	{
		auto pMesh = make_shared<Mesh>();
		pMesh->Set_Name(L"Sphere");
		pMesh->CreateSphere();
		Add(pMesh->Get_Name(), pMesh);
	}
	{
		auto pMesh = make_shared<Mesh>();
		pMesh->Set_Name(L"Point");
		pMesh->CreatePoint();
		Add(pMesh->Get_Name(), pMesh);
	}
}

void ResourceMgr::CreateDefaultShader()
{
	{ 
		wstring ShaderTag = L"Shader_Particle2.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::PARTICLE);
	}

	{
		wstring ShaderTag = L"Shader_Effect2.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::FORWARD);
	}

	{
		wstring ShaderTag = L"Shader_Mesh.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::FORWARD);
	}
	{
		wstring ShaderTag = L"Shader_Model.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::DEFERRED);
	}
	{
		wstring ShaderTag = L"Shader_Effect.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::FORWARD);
	}
	{
		wstring ShaderTag = L"Shader_Particle.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::PARTICLE);
	}
	{
		wstring ShaderTag = L"SkyBox.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::FORWARD);
	}
	{
		wstring ShaderTag = L"Billboard.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::FORWARD);
	}
	{
		wstring ShaderTag = L"Shader_Deferred.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::DEFERRED);
	}	
	{
		wstring ShaderTag = L"Blur.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::DEFERRED);
	}
	{
		wstring ShaderTag = L"Lighting.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
	{
		wstring ShaderTag = L"Shader_Trail.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::FORWARD);
	}
	{
		wstring ShaderTag = L"Shader_Distortion.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::DISTORTION);
	}
	{
		wstring ShaderTag = L"Distorted_Final.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
	{ // MEMO : must
		wstring ShaderTag = L"Shader_Final.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
	{ // MEMO : must
		wstring ShaderTag = L"Shader_Bloom.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
	{ // MEMO : must
		wstring ShaderTag = L"Shader_SSAO.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
	{ // MEMO : must
		wstring ShaderTag = L"Shader_FXAA.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
	{ // MEMO : must
		wstring ShaderTag = L"Shader_LensFlare.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
	{ // MEMO : must
		wstring ShaderTag = L"Shader_Outline.fx";
		Load<Shader>(ShaderTag, ShaderTag);
		auto shader = Get<Shader>(ShaderTag);
		shader->Set_ShaderType(SHADER_TYPE::LIGHTING);
	}
}

void ResourceMgr::CreateModel(const wstring& path)
{
	{
		wstring assetPath = path;

		for (auto& entry : fs::recursive_directory_iterator(assetPath))
		{
			if (entry.is_directory())
				continue;

			if (entry.path().extension().wstring() != L".Model" && entry.path().extension().wstring() != L".model")
				continue;

			wstring tag = entry.path();
			Utils::DetachExt(tag);

			shared_ptr<Model> model = make_shared<Model>();
			model->ReadModel(tag);
			model->ReadMaterial(tag);
			model->ReadAnimation(tag);

			wstring key = fs::path(tag).filename();
			Add(key, model);
		}
	}
}

void ResourceMgr::CreateDefaultMaterial()
{
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Lighting.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"PositionTarget"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"NormalTarget"));
		material->Set_SubMap(2, RESOURCES.Get<Texture>(L"ShadowTarget"));
		material->Set_SubMap(3, RESOURCES.Get<Texture>(L"SSAOUPSCALE2"));
		Add(L"LightMaterial", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Distorted_Final.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BloomFinalTarget"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"DistortionTarget"));
		material->Set_SubMap(2, RESOURCES.Get<Texture>(L"DistortionTarget1"));
		Add(L"Distorted_Final", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Lighting.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);

		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"PositionTarget"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"DiffuseTarget"));
		material->Set_SubMap(2, RESOURCES.Get<Texture>(L"SpecularTarget"));
		material->Set_SubMap(3, RESOURCES.Get<Texture>(L"EmissiveTarget"));
		material->Set_SubMap(4, RESOURCES.Get<Texture>(L"AmbientLightTarget"));
		material->Set_SubMap(5, RESOURCES.Get<Texture>(L"DiffuseLightTarget"));
		material->Set_SubMap(6, RESOURCES.Get<Texture>(L"SpecularLightTarget"));
		material->Set_SubMap(7, RESOURCES.Get<Texture>(L"EmissiveLightTarget"));


		Add(L"LightFinal", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Outline.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"NormalTarget"));
		Add(L"Outline", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Outline.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"OutLineTarget"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"FinalTarget"));
		Add(L"OutlineFinal", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_SSAO.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"NormalTarget"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"G_DepthTarget"));
		material->Set_SubMap(2, RESOURCES.Get<Texture>(L"SSAO_RandomVectorTexture"));
		Add(L"SSAO", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"SSAOTarget"));
		Add(L"SSAODownScale0", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"SSAODOWNSCALE0"));
		Add(L"SSAODownScale1", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"SSAODOWNSCALE1"));
		Add(L"SSAODownScale2", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"SSAODOWNSCALE2"));
		Add(L"SSAOUpScale0", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"SSAOUPSCALE0"));
		Add(L"SSAOUpScale1", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"SSAOUPSCALE1"));
		Add(L"SSAOUpScale2", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BlurTarget"));
		Add(L"BlurSmaller0", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLURSMALLER0"));
		Add(L"BlurSmaller1", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLURSMALLER1"));
		Add(L"BlurBigger0", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLURBIGGER0"));
		Add(L"BlurBigger1", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"VelocityMap"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"FinalTarget"));
		material->Set_SubMap(2, RESOURCES.Get<Texture>(L"G_DepthTarget"));
		Add(L"MotionBlurFinal", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Bloom.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"MotionBlurFinalTarget"));
		Add(L"BloomTarget", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BloomTarget"));
		Add(L"BloomDownScale0", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLOOMDOWNSCALE0"));
		Add(L"BloomDownScale1", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLOOMDOWNSCALE1"));
		Add(L"BloomDownScale2", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLOOMDOWNSCALE2"));
		Add(L"BloomUpScale0", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLOOMUPSCALE0"));
		Add(L"BloomUpScale1", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Blur.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"BLOOMUPSCALE1"));
		Add(L"BloomUpScale2", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Bloom.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"MotionBlurFinalTarget"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"BLOOMUPSCALE2"));
		material->Set_SubMap(2, RESOURCES.Get<Texture>(L"BloomTarget"));
		Add(L"BloomFinal", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_FXAA.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"DistortionFinalTarget"));

		Add(L"FXAARenderFinal", material);
	}

	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_LensFlare.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(L"DistortionFinalTarget"));
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"G_DepthTarget"));

		Add(L"LensFlare", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_LensFlare.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(2, RESOURCES.Get<Texture>(L"LensFlareTarget"));
		Add(L"LensFlareFinal", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Final.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);

		Add(L"AberrationFinal", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Final.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);
		material->Set_SubMap(1, RESOURCES.Get<Texture>(L"G_DepthTarget"));
		Add(L"FogFinal", material);
	}
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Final.fx");
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_Shader(shader);

		Add(L"BackBufferRenderFinal", material);
	}
	
}

void ResourceMgr::CreateMapModel(const wstring& mapName)
{
	wstring assetPath = L"..\\Resources\\Models\\Normal\\Map\\" + mapName;

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".Model" && entry.path().extension().wstring() != L".model")
			continue;

		wstring tag = entry.path().wstring();
		Utils::DetachExt(tag);

		int i = int(tag.rfind(L"Normal\\"));

		tag = tag.substr(i, tag.size() - i);

		shared_ptr<Model> model = make_shared<Model>();
		model->ReadModel(tag);
		model->ReadMaterial(tag);
		model->ReadAnimation(tag);

		Add(tag, model);
	}
}

void ResourceMgr::CreateDefaultFont()
{
	{
		shared_ptr<CustomFont> font = make_shared<CustomFont>();
		font->Load(L"..\\Resources\\Font\\Godic.spritefont");

		Add(L"Godic", font);
	}
	{
		shared_ptr<CustomFont> font = make_shared<CustomFont>();
		font->Load(L"..\\Resources\\Font\\136ex.spritefont");

		Add(L"136ex", font);
	}
	{
		shared_ptr<CustomFont> font = make_shared<CustomFont>();
		font->Load(L"..\\Resources\\Font\\Dream.spritefont");

		Add(L"Dream", font);
	}
}

void ResourceMgr::CreateMeshEffectData()
{
	wstring assetPath = L"..\\Resources\\EffectData\\MeshEffectData\\";

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".dat")
			continue;

		MeshEffectData::DESC tDesc;
		MeshEffectData::Transform_Desc tTransformDesc;

		// For. load file and fill imgui 
		string strFilePath = entry.path().string();
		shared_ptr<FileUtils> file = make_shared<FileUtils>();
		file->Open(Utils::ToWString(strFilePath), FileMode::Read);

		/* Property */
		string strTag = file->Read<string>();
		tDesc.pszTag = strTag.c_str();
		tDesc.fDuration = file->Read<_float>();
		tDesc.bBlurOn = file->Read<_bool>();
		tDesc.bUseFadeOut = file->Read<_bool>();
		tDesc.iMeshCnt = file->Read<_int>();
		tDesc.fCreateInterval = file->Read<_float>();
		tDesc.vParticleDuration = file->Read<_float2>();

		/* Mesh */
		tDesc.strVfxMesh = file->Read<string>();

		/* Coloring Option */
		tDesc.bColorChangingOn = file->Read<_bool>();

		/* Diffuse */
		tDesc.strDiffuseTexture = file->Read<string>();
		tDesc.vDiffuseColor_BaseStart = file->Read<_float4>();
		tDesc.vDiffuseColor_BaseEnd = file->Read<_float4>();
		tDesc.vDestColor_Diffuse = file->Read<_float4>();

		/* Alpha Gradation */
		tDesc.fAlphaGraIntensity = file->Read<_float>();
		tDesc.vBaseColor_AlphaGra = file->Read<_float4>();
		tDesc.vDestColor_AlphaGra = file->Read<_float4>();

		/* Opacity */
		tDesc.strOpacityTexture = file->Read<string>();
		tDesc.iSamplerType = file->Read<_int>();
		tDesc.vTiling_Opacity = file->Read<_float2>();
		tDesc.vUVSpeed_Opacity = file->Read<_float2>();
		
		/* Gradation by Texture */
		tDesc.strGraTexture = file->Read<string>();
		tDesc.vBaseColor_Gra = file->Read<_float4>();
		tDesc.vTiling_Gra = file->Read<_float2>();
		tDesc.vUVSpeed_Gra = file->Read<_float2>();
		tDesc.vDestColor_Gra = file->Read<_float4>();
		
		/* Overlay */
		tDesc.bIsOverlayOn = file->Read<_bool>();
		tDesc.strOverlayTexture = file->Read<string>();
		tDesc.vBaseColor_Overlay = file->Read<_float4>();
		tDesc.vTiling_Overlay = file->Read<_float2>();
		tDesc.vUVSpeed_Overlay = file->Read<_float2>();
		
		/* Normal */
		tDesc.strNormalTexture = file->Read<string>();

		/* Dissolve */
		tDesc.strDissolveTexture = file->Read<string>();
		tDesc.vTiling_Dissolve = file->Read<_float2>();
		tDesc.vUVSpeed_Dissolve = file->Read<_float2>();
		tDesc.bInverseDissolve = file->Read<_bool>();
		
		/* Distortion */
		tDesc.strDistortionTexture = file->Read<string>();
		tDesc.vTiling_Distortion = file->Read<_float2>();
		tDesc.vUVSpeed_Distortion = file->Read<_float2>();

		/* Blend */
		tDesc.strBlendTexture = file->Read<string>();
		
		/* Color Edit */
		tDesc.fContrast = file->Read<_float>();

		// For. Load Transform_Desc 
		/* Init Position */
		tTransformDesc.vPosRange = file->Read<_float3>();

		/* Init scale */
		tTransformDesc.vInitScale_Min = file->Read<_float3>();
		tTransformDesc.vInitScale_Max = file->Read<_float3>();

		/* Init Rotation */
		tTransformDesc.vInitRotation_Min = file->Read<_float3>();
		tTransformDesc.vInitRotation_Max = file->Read<_float3>();

		/* Translate */
		tTransformDesc.iTranslateOption = file->Read<_int>();
		tTransformDesc.fTranslateSpeed = file->Read<_float>();
		tTransformDesc.vEndPosOffset_Min = file->Read<_float3>();
		tTransformDesc.vEndPosOffset_Max = file->Read<_float3>();

		/* Scaling */
		tTransformDesc.iScalingOption = file->Read<_int>();
		tTransformDesc.vEndScale = file->Read<_float3>();

		/* Turn */
		tTransformDesc.iTurnOption = file->Read<_int>();
		tTransformDesc.fTurnSpeed = file->Read<_float>();
		tTransformDesc.vRandomAxis_Min = file->Read<_float3>();
		tTransformDesc.vRandomAxis_Max = file->Read<_float3>();

		shared_ptr<MeshEffectData> meshEffectData = make_shared<MeshEffectData>();
		meshEffectData->Set_Desc(tDesc);
		meshEffectData->Set_TransformDesc(tTransformDesc);

		wstring key = Utils::ToWString(tDesc.pszTag);
		Add(key, meshEffectData);
	}
}

void ResourceMgr::CreateParticleData()
{
}

void ResourceMgr::CreateGroupEffectData()
{
	wstring assetPath = L"..\\Resources\\EffectData\\GroupEffectData\\";

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".dat")
			continue;

		// For. load file and fill imgui 
		string strFilePath = entry.path().string();
		string strFileName = entry.path().filename().string();
		shared_ptr<FileUtils> file = make_shared<FileUtils>();
		file->Open(Utils::ToWString(strFilePath), FileMode::Read);

		vector<GroupEffectData::MemberEffect_Desc> vMemberEffect;

		/* Tag */
		wstring wstrTag = Utils::ToWString(file->Read<string>());
		_int iNumMembers = file->Read<_int>();

		/* Member Effects */
		for (_int i = 0; i < iNumMembers; i++)
		{
			GroupEffectData::MemberEffect_Desc tDesc;

			tDesc.wstrEffectTag = Utils::ToWString(file->Read<string>());
			tDesc.eType = GroupEffectData::EFFECT_TYPE(file->Read<_int>());
			tDesc.fCreateTime = file->Read<_float>();

			tDesc.bIsActive = file->Read<_bool>();

			tDesc.vPivot_Pos = file->Read<_float3>();
			tDesc.vPivot_Scale = file->Read<_float3>();
			tDesc.vPivot_Rotation = file->Read<_float3>();

			vMemberEffect.push_back(tDesc);
		}

		// For. Add ResourceBase to Resource Manager 
		shared_ptr<GroupEffectData> groupEffectData = make_shared<GroupEffectData>();
		groupEffectData->Set_Name(wstrTag);
		groupEffectData->Set_Tag(wstrTag);
		groupEffectData->Set_MemberEffectData(vMemberEffect);

		Add(wstrTag, groupEffectData);
	}
}

void ResourceMgr::Reset_LevelModel(_uint iLevelIndex)
{
	array<KeyResouceMap, RESOURCE_TYPE_COUNT>& resources = m_Resources[iLevelIndex];

	for (auto& resourceMap : resources)
	{
		resourceMap.clear();
	}
}

shared_ptr<Texture> ResourceMgr::CreateTexture(const wstring& name, DXGI_FORMAT format, _uint width, _uint height, _uint BindFlags, _float4 clearColor)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->Create(format, width, height, BindFlags, clearColor);
	Add(name, texture);

	return texture;

}

shared_ptr<Texture> ResourceMgr::CreateTextureFromResource(const wstring& path, ComPtr<ID3D11Texture2D> texture)
{
	shared_ptr<Texture> text = make_shared<Texture>();
	text->CreateFromResource(texture);
	Add(path, text);
	return text;
}

shared_ptr<Texture> ResourceMgr::CreateShadowTexture(const wstring& name, _float width, _float height)
{
	shared_ptr<Texture> text = make_shared<Texture>();
	text->CreateShadowDST(width, height);
	Add(name, text);
	return text;
}

