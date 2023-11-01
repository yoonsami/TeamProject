#pragma once
#include "Component.h"

class Shader;
class Mesh;
class MeshCollider;
enum class ColliderType
{
    Sphere,
    AABB,
    OBB,
    Mesh
};

class BaseCollider :  public Component , public enable_shared_from_this<BaseCollider>
{
public:
    BaseCollider(ColliderType colliderType);
    virtual ~BaseCollider();

public:
    virtual HRESULT Init() = 0;
    virtual _bool Intersects(Ray& worldRay, OUT _float& distance) = 0;
    virtual _bool Intersects(shared_ptr<BaseCollider>& others) = 0;
    virtual void Render();
	virtual void Render_Instancing(shared_ptr<InstancingBuffer>& buffer);

public:
    virtual TransformDesc Get_TransformDesc() = 0 ;
    virtual InstanceID Get_InstanceID() { return make_pair(_ulonglong(m_eColliderType), _ulonglong(m_eColliderType)); }
    virtual _float3 Get_ClosestPoint(shared_ptr<BaseCollider> pCollider) = 0;
    virtual _float3 Get_Extend() = 0;
    virtual _float3 Get_CenterPos() = 0;
    vector<shared_ptr<Mesh>>& Get_Meshes() { return m_Meshes; }
    _uint		Get_ID() { return m_iID; }

    ColliderType Get_ColliderType() { return m_eColliderType; }

    void Set_Offset(const _float3& offset) { m_vOffset = offset; }
    _float3& Get_Offset() { return m_vOffset; }

    virtual _bool Intersects_With_Bottom(shared_ptr<MeshCollider>& others, OUT _float3& collidePos, OUT _float& dist) = 0;

    void Set_CollisionGroup(_uint index) { m_eCollisionGroup = index; }
    _uint Get_CollisionGroup() { return m_eCollisionGroup; }

    _bool Is_Activated() { return m_bActivated; }
    void Set_Activate(_bool flag) { m_bActivated = flag; }

	void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap);
	void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap);
	void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap);

    void Set_Function(function<void(void)> func) { m_pResultFunc = func; }

protected:
    void Add_ColliderGroup();

protected:
    ColliderType m_eColliderType;
    _float3 m_vOffset = { 0,0,0 };
    shared_ptr<Shader> DebugShader;
    vector<shared_ptr<Mesh>> m_Meshes;

protected:
    _uint m_iID = 0;
    _int m_iColCount = 0;
    static _uint g_iNextID;
    _uint m_eCollisionGroup = MAX_COLLIDER_GROUP;
    _bool m_bActivated = true;
    _float3 m_vExtent = _float3(1.f);

    function<void(void)> m_pResultFunc = nullptr;
};

