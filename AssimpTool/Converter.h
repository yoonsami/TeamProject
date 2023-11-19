#pragma once
#include "AsType.h"
#include "Parts.h"
class Converter
{
	friend class MainApp;
public:
	Converter();
	~Converter();

public:
	void ReadAssetFile(const wstring& fileName);
	void ExportModelData(const wstring& savePath);
	void ExportMaterialData(const wstring& savePath);
	void ExportAnimationData(const wstring& savePath);
	//void ReadPartsAnimationData(const wstring& savePath);
	void ReadAdditionalAnimData(const wstring& savePath);
	void ExportFinalAnimData(const wstring& savePath);
	//void ExportPartsAnimationData();

	void ExportBaseData(const wstring& savePath);
	void ExportPartsData(const wstring& savePath, PARTS_INFO eType);

private:
	void ReadModelData(aiNode* node, _int index, _int parent);
	void ReadBaseData(aiNode* node, _int index, _int parent);
	void ReadMeshData(aiNode* node, _int bone);
	void ReadSkinData();

	void WriteCSVFile();
	void WriteModelFile(const wstring& finalPath);
	void WriteBaseFile(const wstring& finalPath);
	void WritePartsFile(const wstring& finalPath, PARTS_INFO eType);

	void ReadPartsModelData();
	void ReadPartsSkinData();

private:
	void ReadMaterialData();
	void WriteMaterialData(const wstring& finalPath);
	void WriteTexture(const wstring& savePath);

private:
	shared_ptr<asAnimation> ReadAnimationData(aiAnimation* srcAnimation);
	shared_ptr<asAnimationNode> ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode);
	void ReadKeyFrameData(shared_ptr<asAnimation> animation, aiNode* node, map<string, shared_ptr<asAnimationNode>>& cache);
	void WriteAnimationData(const wstring& finalPath);

private:
	_int Get_BoneIndex(const string& name);

	void ApplyScaleToNode(aiNode* node, const aiMatrix4x4& scale);
	void ApplyScaleToAnimations(const aiMatrix4x4& scale);

private:
	wstring m_strAssetPath = L"../Resources/Assets/";
	wstring m_strModelPath = L"../Resources/Models/";
	wstring m_strTexturePath = L"../Resources/Textures/";

private:
	vector<shared_ptr<asMaterial>>	m_Material;
	vector<shared_ptr<asBone>>		m_Bone;
	vector<shared_ptr<asMesh>>		m_Mesh;
	vector<shared_ptr<asAnimation>> m_Animations;
private:
	shared_ptr<Assimp::Importer> m_pImporter;
	const aiScene* m_pScene;
};

