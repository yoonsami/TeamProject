#include "pch.h"
#include "HIntEffectScript.h"
#include "CreateGroupEffectScript.h"
#include "Smoke_WitcherSense.h"

HRESULT HIntEffectScript::Init()
{
    // Reset 
    return S_OK;
}

void HIntEffectScript::Tick()
{
    // update cooltime 
    for (_int i = 0; i < 21; i++)
        m_fHintEffectTimer[i] += fDT;

    // create effect
    wstring wstrEffectTag = L"AHint_";
    for (_int i = 1; i <= 21; i++)
    {
        // check cooltime 
        if (m_fHintEffectTimer[i - 1] > (2.f + i) && !m_bIsActivated[i - 1])
        {
            // make effect name 
            _int iIndex = i;
            if (i == 21)
                iIndex = 4;
            else if (i >= 4)
                iIndex += 1;
            wstring wstrTargetTag = wstrEffectTag + to_wstring(iIndex);

			shared_ptr<GameObject> pGroupEffectOwnerObj = make_shared<GameObject>();
			pGroupEffectOwnerObj->Set_TimeSlowed(false);

			// For. Transform 
			pGroupEffectOwnerObj->GetOrAddTransform();

			// For. GroupEffect component 
			shared_ptr<CreateGroupEffectScript> pCreateGroupEffect = make_shared<CreateGroupEffectScript>();
			pGroupEffectOwnerObj->Add_Component(pCreateGroupEffect);
			pGroupEffectOwnerObj->Get_Script<CreateGroupEffectScript>()->Set_EffectTag(wstrTargetTag);

			// For. Add Effect GameObject to current scene
			EVENTMGR.Create_Object(pGroupEffectOwnerObj);

            // reset timer 
            m_bIsActivated[i - 1] = true;
        }
    }

    // check all dead 
    _bool   bIsAllActivated = true;
    for (_int i = 0; i < 21; i++)
    {
        if (false == m_bIsActivated[i])
            bIsAllActivated = false;
    }
    if (bIsAllActivated)
    {
        EVENTMGR.Delete_Object(Get_Owner());
        return;
    }
}
