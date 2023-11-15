#include "pch.h"
#include "DemoCameraScript1.h"

void DemoCameraScript1::Tick()
{
	// weak_ptr expired() -> 이 포인터가 이미 소멸해서 사용할 수 없는 상태면 true 반환
	if (m_pOwner.expired())
		return;

	if (KEYPUSH(KEY_TYPE::W))
		Get_Transform()->Go_Straight();

	if (KEYPUSH(KEY_TYPE::S))
		Get_Transform()->Go_Backward();

	if(KEYPUSH(KEY_TYPE::A))
		Get_Transform()->Go_Left();

	if(KEYPUSH(KEY_TYPE::D))
		Get_Transform()->Go_Right();

	if (KEYPUSH(KEY_TYPE::LSHIFT))
	{
		m_pOwner.lock()->Get_Transform()->Set_Speed(50.f);
	}
	else
	{
		m_pOwner.lock()->Get_Transform()->Set_Speed(10.f);
	}
}
