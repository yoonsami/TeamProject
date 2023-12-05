#include "pch.h"
#include "Hide_OrctongScript.h"
#include "Camera.h"
#include <MathUtils.h>
#include "ObjectDissolve.h"
#include "Model.h"
#include "ModelRenderer.h"

void Hide_OrctongScript::Tick()
{
	if (m_pOwner.expired())
		return;

	auto pPlayer = CUR_SCENE->Get_GameObject(L"Player");
	if (!pPlayer)
		return;

	_float3 vOwnerPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vPlayerPos = pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz();

	_float lengthSQ = (vOwnerPos - vPlayerPos).LengthSquared();

	if (lengthSQ >= 3.f * 3.f)
	{
		Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1] = Color(0.f);
		return;
	}

	Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1] = Color(1.f,0.f,0.f,0.f);

	if (!KEYPUSH(KEY_TYPE::E))
		return;

	auto camera = CUR_SCENE->Get_MainCamera()->Get_Camera();
	
	if (camera->Get_Frustum().Contain_Sphere(Get_Owner()->Get_CullPos(), Get_Owner()->Get_CullRadius()))
	{
		auto script = make_shared<ObjectDissolve>(0.3f,RESOURCES.GetOrAddTexture(L"Tex_Cloud_01.tga",L"..\\Resources\\Textures\\Universal\\Tex_Cloud_01.tga"));
		Get_Owner()->Add_Component(script);
		script->Init();
	}
	
	

}
