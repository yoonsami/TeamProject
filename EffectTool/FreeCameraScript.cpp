#include "pch.h"
#include "FreeCameraScript.h"

void FreeCameraScript::Tick()
{
	if (m_pOwner.expired())
		return;
	
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
}
