#include "pch.h"
#include "NPC_FSM.h"
#include "Camera.h"

_bool NPC_FSM::Can_Interact()
{
	if (!KEYTAP(KEY_TYPE::E))
		return false;

	_float3 pos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	if (_float3::Transform(pos, CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ViewMat()).z < 0)
		return false;

	auto pPlayer = CUR_SCENE->Get_GameObject(L"Player");

	if (pPlayer)
	{
		_float lengthSQ = (pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz() - pos).LengthSquared();
		
		if (lengthSQ < 3.f * 3.f)
			return true;
	}


	return false;
}
