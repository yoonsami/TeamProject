#include "pch.h"
#include "GroupEffectOwner.h"
#include "MapObjectLoopEffectScript.h"
MapObjectLoopEffectScript::MapObjectLoopEffectScript(_float _fLoopDuration, wstring _wstrEffectGroup) :
	m_fLoopDuration(_fLoopDuration),
	m_wstrEffectGroup(_wstrEffectGroup)
{
}

void MapObjectLoopEffectScript::Tick()
{
	m_fLoopDelta += fDT;
	if (m_fLoopDelta >= m_fLoopDuration)
	{
		m_fLoopDelta = 0.f;
		// 이펙트 생성 및 재생
        Add_GroupEffectOwner(m_wstrEffectGroup, _float3{ 0.f, 3.f, 0.f }, false, nullptr);
	}
}

void MapObjectLoopEffectScript::Add_GroupEffectOwner(const wstring& strSkilltag, _float3 vPosOffset, _bool usePosAs, shared_ptr<MonoBehaviour> pScript)
{
    shared_ptr<GameObject> pGroupEffectOwnerObj = make_shared<GameObject>();

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