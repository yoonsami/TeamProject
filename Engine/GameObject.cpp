#include "pch.h"
#include "GameObject.h"

#include "Transform.h"
#include "MonoBehaviour.h"

#include "Camera.h"
#include "MeshRenderer.h"
#include "ModelAnimator.h"
#include "Light.h"
#include "ModelRenderer.h"
#include "BaseCollider.h"
#include "FSM.h"
//#include "Physics.h"
#include "BaseUI.h"
#include "CharacterController.h"
//#include "Billboard.h"
#include "RigidBody.h"
//#include "EffectRenderer.h"
#include "ParticleSystem.h"
#include "TrailRenderer.h"
//#include "EffectController.h"
#include "FontRenderer.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

HRESULT GameObject::Init()
{
	HRESULT hr = 0;
	for (auto& component : m_Components)
	{
		if (component)
		{
			hr = component->Init();

			if (hr == E_FAIL)
				return E_FAIL;
		}
	}
	for (auto& script : m_Scripts)
	{
		hr = script->Init();
		if (hr == E_FAIL)
			return E_FAIL;
	}
	return S_OK;
}

void GameObject::Tick()
{
	for (auto& component : m_Components)
	{
		if(component)
			component->Tick();
	}
	auto scripts = m_Scripts;
	for (auto& script : scripts)
		script->Tick();
}

void GameObject::Late_Tick()
{
	for (auto& component : m_Components)
	{
		if (component)
			component->Late_Tick();
	}
	auto scripts = m_Scripts;
	for (auto& script : scripts)
		script->Late_Tick();
}

void GameObject::Final_Tick()
{
	for (auto& component : m_Components)
	{
		if (component)
			component->Final_Tick();
	}
	auto scripts = m_Scripts;
	for (auto& script : scripts)
	{
		script->Final_Tick();
	}
}

void GameObject::Fixed_Tick()
{
	for (auto& component : m_Components)
	{
		if (component)
			component->Fixed_Tick();
	}
	for (auto& script : m_Scripts)
		script->Fixed_Tick();
}

HRESULT GameObject::Add_Component(shared_ptr<Component> component)
{
	component->Set_Owner(shared_from_this());
	_uchar iIndex = static_cast<_uchar>(component->Get_Type());
	if (iIndex < FIXED_COMPONENT_COUNT)
	{
		if (m_Components[iIndex])
			return E_FAIL;

		m_Components[iIndex] = component;
	}
	else
		m_Scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	
	return S_OK;
}

SHADER_TYPE GameObject::Get_ShaderType()
{
	if (Get_MeshRenderer() && Get_MeshRenderer()->Get_Shader())
		return Get_MeshRenderer()->Get_Shader()->Get_ShaderType();
	
	if (Get_ModelRenderer() && Get_ModelRenderer()->Get_Shader())
		return Get_ModelRenderer()->Get_Shader()->Get_ShaderType();

	if (Get_Animator() && Get_Animator()->Get_Shader())
		return Get_Animator()->Get_Shader()->Get_ShaderType();

	if (Get_ParticleSystem() && Get_ParticleSystem()->Get_Shader())
		return Get_ParticleSystem()->Get_Shader()->Get_ShaderType();

	/*if (Get_EffectRenderer() && Get_EffectRenderer()->Get_Shader())
		return Get_EffectRenderer()->Get_Shader()->Get_ShaderType();*/

	if (Get_TrailRenderer() && Get_TrailRenderer()->Get_Shader())
		return Get_TrailRenderer()->Get_Shader()->Get_ShaderType();

	return SHADER_TYPE::NONE;
}

shared_ptr<Component> GameObject::Get_FixedComponent(COMPONENT_TYPE type)
{
	_uchar index = static_cast<_uchar>(type);
	
	return m_Components[index];
}

shared_ptr<CharacterController> GameObject::Get_CharacterController()
{
	return static_pointer_cast<CharacterController>(Get_FixedComponent(COMPONENT_TYPE::CharacterController));
}

shared_ptr<Transform> GameObject::Get_Transform()
{
	return static_pointer_cast<Transform>(Get_FixedComponent(COMPONENT_TYPE::Transform));
}

shared_ptr<Camera> GameObject::Get_Camera()
{
	return static_pointer_cast<Camera>(Get_FixedComponent(COMPONENT_TYPE::Camera));
}

shared_ptr<ModelAnimator> GameObject::Get_Animator()
{
	return static_pointer_cast<ModelAnimator>(Get_FixedComponent(COMPONENT_TYPE::Animator));
}

shared_ptr<Light> GameObject::Get_Light()
{
	return static_pointer_cast<Light>(Get_FixedComponent(COMPONENT_TYPE::Light));
}

shared_ptr<ModelRenderer> GameObject::Get_ModelRenderer()
{
	return static_pointer_cast<ModelRenderer>(Get_FixedComponent(COMPONENT_TYPE::ModelRenderer));
}

shared_ptr<BaseCollider> GameObject::Get_Collider()
{
	return static_pointer_cast<BaseCollider>(Get_FixedComponent(COMPONENT_TYPE::Collider));
}

shared_ptr<FSM> GameObject::Get_FSM()
{
	return static_pointer_cast<FSM>(Get_FixedComponent(COMPONENT_TYPE::FSM));
}

//shared_ptr<Physics> GameObject::Get_Physics()
//{
//	return static_pointer_cast<Physics>(Get_FixedComponent(COMPONENT_TYPE::Physics));
//}
//
shared_ptr<BaseUI> GameObject::Get_Button()
{
	return static_pointer_cast<BaseUI>(Get_FixedComponent(COMPONENT_TYPE::BaseUI));
}

shared_ptr<MeshRenderer> GameObject::Get_MeshRenderer()
{
	return static_pointer_cast<MeshRenderer>(Get_FixedComponent(COMPONENT_TYPE::MeshRenderer));
}

shared_ptr<Transform> GameObject::GetOrAddTransform()
{
	if (!Get_Transform())
	{
		shared_ptr<Transform> transform = make_shared<Transform>();
		Add_Component(transform);
	}

	return Get_Transform();
}

//shared_ptr<Billboard> GameObject::Get_Billboard()
//{
//	return static_pointer_cast<Billboard>(Get_FixedComponent(COMPONENT_TYPE::Billboard));
//
//}
//
shared_ptr<RigidBody> GameObject::Get_RigidBody()
{
	return static_pointer_cast<RigidBody>(Get_FixedComponent(COMPONENT_TYPE::RigidBody));
}

//shared_ptr<EffectRenderer> GameObject::Get_EffectRenderer()
//{
//	return static_pointer_cast<EffectRenderer>(Get_FixedComponent(COMPONENT_TYPE::EffectRenderer));
//}
//
shared_ptr<ParticleSystem> GameObject::Get_ParticleSystem()
{
	return static_pointer_cast<ParticleSystem>(Get_FixedComponent(COMPONENT_TYPE::ParticleSystem));
}

shared_ptr<TrailRenderer> GameObject::Get_TrailRenderer()
{
	return static_pointer_cast<TrailRenderer>(Get_FixedComponent(COMPONENT_TYPE::TrailRenderer));
}

//shared_ptr<EffectController> GameObject::Get_EffectController()
//{
//	return static_pointer_cast<EffectController>(Get_FixedComponent(COMPONENT_TYPE::EffectController));
//}
//
shared_ptr<FontRenderer> GameObject::Get_FontRenderer()
{
	return static_pointer_cast<FontRenderer>(Get_FixedComponent(COMPONENT_TYPE::FontRenderer));
}

shared_ptr<Model> GameObject::Get_Model()
{
	if (Get_Animator())
		return Get_Animator()->Get_Model();
	if (Get_ModelRenderer())
		return Get_ModelRenderer()->Get_Model();

	return nullptr;
}
