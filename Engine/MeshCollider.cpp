#include "pch.h"
#include "MeshCollider.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Geometry.h"
#include "Camera.h"
#include "Utils.h"

MeshCollider::MeshCollider(wstring modelTag, _float4x4 MatWorld)
    :BaseCollider(ColliderType::Mesh)
{
    m_strModelTag = modelTag;
    shared_ptr<Model> model = RESOURCES.Get<Model>(modelTag);
    if (!model)
        assert(false);

    m_pModel = model;
    auto& meshes = model->Get_Meshes();
	auto world = MatWorld;

	shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();

	const _uint boneCount = model->Get_BoneCount();

	for (_uint i = 0; i < boneCount; ++i)
	{
		shared_ptr<ModelBone> bone = model->Get_BoneByIndex(i);
		boneDesc->transform[i] = bone->transform * Utils::m_matNonAnimPivot;
	}

	for (auto& mesh : meshes)
	{
        _int boneIndex = mesh->boneIndex;
        auto& vertices = mesh->geometry->Get_Vertices();
        auto& indices = mesh->geometry->Get_Indices();

        shared_ptr<Mesh> colliderMesh = make_shared<Mesh>();
        auto geometry = colliderMesh->Get_Geometry();

        for (auto& vertex : vertices)
        {
            _float3 vertexPos = vertex.vPosition;
            vertexPos = _float3::Transform(vertexPos, boneDesc->transform[boneIndex] );
          
            VTXTEXNORTANDATA vtx;
            vtx.vPosition = vertexPos;
          
            geometry->Add_Vertex(vtx);
        }
        geometry->Add_Indices(indices);
        colliderMesh->Create_Buffer();


        m_Meshes.push_back(colliderMesh);
		
	}

   

}

MeshCollider::~MeshCollider()
{
}

HRESULT MeshCollider::Init()
{
    return S_OK;
}

void MeshCollider::Tick()
{
}

_bool MeshCollider::Intersects(Ray& ray, OUT _float& distance)
{
    _float minDist = FLT_MAX;

    _bool bCollided = false;
    _float4x4 matWorld = Get_Transform()->Get_WorldMatrix();

    for (auto& mesh : m_Meshes)
    {
        auto& vertices = mesh->Get_Geometry()->Get_Vertices();
        auto& indices = mesh->Get_Geometry()->Get_Indices();

        for (_int i = 0; i < _int(indices.size());)
        {
            _float fdistance = 0;
            _float3 vVtxPos[3] = {
                _float3::Transform(vertices[indices[i++]].vPosition,matWorld),
                _float3::Transform(vertices[indices[i++]].vPosition,matWorld),
                _float3::Transform(vertices[indices[i++]].vPosition,matWorld)
            };

			if (!ray.Intersects(vVtxPos[0],vVtxPos[1],vVtxPos[2], fdistance))
				continue;

			if (fdistance < minDist)
			{
				bCollided = true;
				minDist = fdistance;
				distance = minDist;
			}

        }
    }

    return bCollided;
}

_bool MeshCollider::Intersects(shared_ptr<BaseCollider>& others)
{
    return false;
}

InstanceID MeshCollider::Get_InstanceID()
{
    return make_pair(_ulonglong(m_pModel.get()), _ulonglong(m_eType));
}

TransformDesc MeshCollider::Get_TransformDesc()
{
    return TransformDesc{ Get_Transform()->Get_WorldMatrix() };
}

_bool MeshCollider::Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& dist)
{
    return _bool();
}

_float3 MeshCollider::Get_ClosestPoint(shared_ptr<BaseCollider> pCollider)
{
    return _float3();
}
