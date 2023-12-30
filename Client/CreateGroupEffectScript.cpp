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
			shared_ptr<GameObject> pGroupEffectOwnerObj = make_shared<GameObject>();
			pGroupEffectOwnerObj->Set_TimeSlowed(false);
			// For. Transform 
			pGroupEffectOwnerObj->GetOrAddTransform();
		
			// For. GroupEffect component 
			shared_ptr<GroupEffectOwner> pGroupEffect = make_shared<GroupEffectOwner>();
			pGroupEffectOwnerObj->Add_Component(pGroupEffect);
			pGroupEffectOwnerObj->Get_GroupEffectOwner()->Set_GroupEffectTag(m_wstrEffectTag);
			
			// TODO : add script 

			// For. Add Effect GameObject to current scene
			EVENTMGR.Create_Object(pGroupEffectOwnerObj);
			m_fHintEffectTimer = 0.f;
		}
	}
	else
	{
		EVENTMGR.Delete_Object(Get_Owner());
		return;
	}
}
