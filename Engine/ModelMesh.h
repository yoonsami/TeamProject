#pragma once

struct ModelBone
{
	~ModelBone() {


	}
	wstring name;
	_int index;
	_int parentIndex;
	weak_ptr<ModelBone> parent;

	_float4x4 transform;
	vector<weak_ptr<ModelBone>> children;

	_float4x4 offsetTransform;
};

struct ModelMesh
{
	shared_ptr<ModelMesh> Clone();
	void Create_Buffer();
	wstring name;

	shared_ptr<Geometry<ModelVertexType>> geometry = make_shared<Geometry<ModelVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;

	wstring materialName = L"";
	weak_ptr<Material> material; // cache

	_int boneIndex;
	weak_ptr<ModelBone> bone; // cache
};

