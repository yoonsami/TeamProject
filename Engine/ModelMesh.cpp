#include "pch.h"
#include "ModelMesh.h"

#include "Geometry.h"

shared_ptr<ModelMesh> ModelMesh::Clone()
{
	shared_ptr<ModelMesh> newMesh = make_shared<ModelMesh>();

	newMesh->boneIndex = boneIndex;
	newMesh->geometry = geometry;
	newMesh->vertexBuffer = vertexBuffer;
	newMesh->indexBuffer = indexBuffer;
	newMesh->name = name;
	newMesh->materialName = materialName;




	return newMesh;
}

void ModelMesh::Create_Buffer()
{
	vertexBuffer = make_shared<VertexBuffer>();
	vertexBuffer->Create(geometry->Get_Vertices());
	indexBuffer = make_shared<IndexBuffer>();
	indexBuffer->Create(geometry->Get_Indices());
}
