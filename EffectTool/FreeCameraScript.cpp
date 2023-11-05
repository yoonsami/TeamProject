#include "pch.h"
#include "FreeCameraScript.h"

void FreeCameraScript::Tick()
{
	if (m_pOwner.expired())
		return;

	if (KEYHOLD(KEY_TYPE::LBUTTON))
	{

		// For. Translate 
		if (KEYPUSH(KEY_TYPE::W))
			Get_Transform()->Go_Straight();
		if (KEYPUSH(KEY_TYPE::A))
			Get_Transform()->Go_Left();
		if (KEYPUSH(KEY_TYPE::S))
			Get_Transform()->Go_Backward();
		if (KEYPUSH(KEY_TYPE::D))
			Get_Transform()->Go_Right();
		if (KEYPUSH(KEY_TYPE::Q))
			Get_Transform()->Go_Horizontally_Up();
		if (KEYPUSH(KEY_TYPE::E))
			Get_Transform()->Go_Horizontally_Down();

		// For. Roataion 
		if (KEYHOLD(KEY_TYPE::LCTRL))
		{
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
	}

}
