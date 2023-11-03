#include "pch.h"
#include "DemoCameraScript2.h"

void DemoCameraScript2::Tick()
{
	if (!KEYPUSH(KEY_TYPE::LCTRL))
		return;
		
	_float2 vMouseDir = INPUT.GetMouseDir();

	{
		_float3 axis = _float3::Up;

		Get_Transform()->Turn(axis, vMouseDir.x * m_fRotationSpeed);
	}
	{
		_float3 axis = Get_Transform()->Get_State(Transform_State::RIGHT).xyz();

		Get_Transform()->Turn(axis, vMouseDir.y * m_fRotationSpeed);
	}
}
