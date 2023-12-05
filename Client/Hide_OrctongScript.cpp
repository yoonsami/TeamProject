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
		return;


	if (!KEYPUSH(KEY_TYPE::E))
		return;

	auto camera = CUR_SCENE->Get_MainCamera()->Get_Camera();
	
	if (camera->Get_Frustum().Contain_Sphere(Get_Owner()->Get_CullPos(), Get_Owner()->Get_CullRadius()))
	{
		auto script = make_shared<ObjectDissolve>(0.3f);
		Get_Owner()->Add_Component(script);
		script->Init();
	}
	
	

}
