#pragma once
#include "Component.h"
class FSM;
class Model;
class Light;
class BaseUI;
class Camera;
class Physics;
class RigidBody;
class Transform;
class Billboard;
class BaseCollider;
class MeshRenderer;
class MonoBehaviour;
class ModelAnimator;
class ModelRenderer;
class EffectRenderer;
class CharacterController;
class ParticleSystem;
class Particle;
class MeshEffect;
class TrailRenderer;
class EffectController;
class FontRenderer;
class MotionTrailRenderer;

enum LayerMask
{
	Layer_Default = 0,
	Layer_UI = 1,
	Layer_Skybox = 2,
	Layer_Effect = 3,
};

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

public:
	virtual HRESULT Init();
	virtual void Tick() ;
	virtual void Late_Tick() ;
	virtual void Final_Tick();
	virtual void Fixed_Tick();

public:
	virtual HRESULT Add_Component(shared_ptr<Component> component) final;
	virtual HRESULT Change_Component(shared_ptr<Component> component) final;

public:
	const wstring& Get_Name() { return m_strName; }
	void Set_Name(const wstring& name) { m_strName = name; }
	_uchar Get_LayerIndex() { return m_iLayerIndex; }
	void Set_LayerIndex(_uchar layerIndex) { m_iLayerIndex = layerIndex; }
	_bool Get_Instancing() { return m_bInstancing; }
	void Set_Instancing(_bool flag) { m_bInstancing = flag; }

	_uint Get_ObjectGroup() { return m_eObjectGroup; }
	void Set_ObjectGroup(_uint index) { m_eObjectGroup = index; }


	_bool Is_Blured() { return m_bIsBlur; }
	void Set_Blur(_bool flag) { m_bIsBlur = flag; }

	_bool Has_VelocityMap() { return m_bHasVelocityMap; }
	void Set_VelocityMap(_bool flag) { m_bHasVelocityMap = flag; }

	SHADER_TYPE Get_ShaderType();
	vector<shared_ptr<MonoBehaviour>>& Get_Scripts() { return m_Scripts; }
	shared_ptr<Component> Get_FixedComponent(COMPONENT_TYPE type);
	shared_ptr<CharacterController> Get_CharacterController();
	shared_ptr<Transform> Get_Transform();
	shared_ptr<MeshRenderer> Get_MeshRenderer();
	shared_ptr<Camera> Get_Camera();
	shared_ptr<ModelAnimator> Get_Animator();
	shared_ptr<Light> Get_Light();
	shared_ptr<ModelRenderer> Get_ModelRenderer();
	shared_ptr<BaseCollider> Get_Collider();
	shared_ptr<FSM> Get_FSM();
	//shared_ptr<Physics> Get_Physics();
	shared_ptr<BaseUI> Get_Button();
	shared_ptr<Transform> GetOrAddTransform();
	//shared_ptr<Billboard> Get_Billboard();
	shared_ptr<RigidBody> Get_RigidBody();
	//shared_ptr<EffectRenderer> Get_EffectRenderer();
	shared_ptr<ParticleSystem> Get_ParticleSystem();
	shared_ptr<Particle> Get_Particle();
	shared_ptr<MeshEffect> Get_Effect();
	shared_ptr<TrailRenderer> Get_TrailRenderer();
	//shared_ptr< EffectController> Get_EffectController();
	shared_ptr<FontRenderer> Get_FontRenderer();
	shared_ptr<MotionTrailRenderer> Get_MotionTrailRenderer();

	shared_ptr<Model> Get_Model();
	
	_float3& Get_CullPos() { return m_vCullPos; }
	_float Get_CullRadius() { return m_fCullRaidus; }
	void Set_CullRadius(_float radius) { m_fCullRaidus = radius; }
	_bool Is_FrustumCulled() { return m_bFrustumCull; }
	void Set_FrustumCulled(_bool flag) { m_bFrustumCull = flag; }

	void Set_DrawShadow(_bool flag) { m_bDrawShadow = flag; }
	_bool Is_DrawShadow() { return m_bDrawShadow; }

	_float& Get_CurHp() { return m_fHP; }
	_float Get_HpRatio() { return m_fHP / m_fMaxHP; }
	void Set_Hp(_float fHP) { m_fHP = fHP; }
	void Set_MaxHp(_float fHP) { m_fMaxHP = fHP; m_fHP = fHP; }

	_bool Get_Hurt(_float fDamage)
	{
		m_fHP -= fDamage;
		if (m_fHP <= 0)
		{
			m_fHP = 0;
			return true;
		}
		else
			return false;
	}

	template<typename T>
	shared_ptr<T> Get_Script();

private:
	wstring m_strName = L"";
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> m_Components;
	vector<shared_ptr<MonoBehaviour>> m_Scripts;

	_bool m_bInstancing = true;
	_uchar m_iLayerIndex = Layer_Default;
	_uint m_eObjectGroup = 0;

	_bool m_bFrustumCull = false;
	_float3 m_vCullPos = { 0,0,0 };
	_float m_fCullRaidus = 1.f;

	_bool m_bDrawShadow = false;

	_float m_fHP = 0.f;
	_float m_fMaxHP = 100.f;
	_bool m_bIsBlur = false;
	_bool m_bHasVelocityMap = false;
};

template<typename T>
inline shared_ptr<T> GameObject::Get_Script()
{
	for (auto& script : Get_Scripts())
	{
		if (typeid(*script.get()) == typeid(T))
			return static_pointer_cast<T>(script);
	}
	return nullptr;
}
