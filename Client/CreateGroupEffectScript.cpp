#include "pch.h"
#include "CreateGroupEffectScript.h"
#include "GroupEffectOwner.h"

HRESULT CreateGroupEffectScript::Init()
{
    return S_OK;
}

void CreateGroupEffectScript::Tick()
{
	m_fHintEffectTimer += fDT;

	// check distance
	if (m_bIsAvailable)
	{
		// create effect 
		if (m_fHintEffectTimer > 7.f)
		{
			Add_GroupEffectOwner(m_wstrEffectTag, _float3(0.f, 0.f, 0.f), true, nullptr, false);
			m_fHintEffectTimer = 0.f;
		}
	}
	else
	{
		EVENTMGR.Delete_Object(Get_Owner());
		return;
	}
}

void CreateGroupEffectScript::Add_GroupEffectOwner(const wstring& strSkilltag, _float3 vPosOffset, _bool usePosAs, shared_ptr<MonoBehaviour> pScript, _bool bTimeSlowed)
{
	shared_ptr<GameObject> pGroupEffectOwnerObj = make_shared<GameObject>();
	pGroupEffectOwnerObj->Set_TimeSlowed(bTimeSlowed);
	// For. Transform 
	pGroupEffectOwnerObj->GetOrAddTransform();
	if (!usePosAs)
	{
		_float4 vOwnerLook = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::LOOK);
		vOwnerLook.Normalize();
		_float4 vOwnerRight = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::RIGHT);
		vOwnerRight.Normalize();
		_float4 vOwnerUp = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::UP);
		vOwnerUp.Normalize();
		_float4 vOwnerPos = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS)
			+ vOwnerRight * vPosOffset.x
			+ vOwnerUp * vPosOffset.y
			+ vOwnerLook * vPosOffset.z;
		pGroupEffectOwnerObj->Get_Transform()->Set_State(Transform_State::POS, vOwnerPos);
	}
	else
		pGroupEffectOwnerObj->Get_Transform()->Set_State(Transform_State::POS, _float4(vPosOffset, 1.f));
	pGroupEffectOwnerObj->Get_Transform()->Set_Quaternion(Get_Transform()->Get_Rotation());

	// For. GroupEffect component 
	shared_ptr<GroupEffectOwner> pGroupEffect = make_shared<GroupEffectOwner>();
	pGroupEffectOwnerObj->Add_Component(pGroupEffect);
	pGroupEffectOwnerObj->Get_GroupEffectOwner()->Set_GroupEffectTag(strSkilltag);
	if (nullptr != pScript)
		pGroupEffectOwnerObj->Get_GroupEffectOwner()->Set_GroupEffectScript(pScript);

	// For. Add Effect GameObject to current scene
	EVENTMGR.Create_Object(pGroupEffectOwnerObj);
}