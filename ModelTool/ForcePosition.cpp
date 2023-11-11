#include "pch.h"
#include "ForcePosition.h"
#include "ModelAnimator.h"

void ForcePosition::Final_Tick()
{
	if(bForcePosition)
		Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));


	if (Get_Owner()->Get_Animator()->Is_Finished())
		Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
}
