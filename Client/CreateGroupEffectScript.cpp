#include "pch.h"
#include "CreateGroupEffectScript.h"
#include "GroupEffect.h"

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
			shared_ptr<GameObject> pGroupEffectObj = make_shared<GameObject>();
			pGroupEffectObj->Set_TimeSlowed(false);
			
			// For. Transform 
			pGroupEffectObj->GetOrAddTransform();
			
			// For. GroupEffectData 
			wstring wstrFileName = m_wstrEffectTag + L".dat";
			wstring wtsrFilePath = TEXT("..\\Resources\\EffectData\\GroupEffectData\\") + wstrFileName;
			shared_ptr<GroupEffectData> pGroupEffectData = RESOURCES.GetOrAddGroupEffectData(m_wstrEffectTag, wtsrFilePath);

			if (pGroupEffectData == nullptr)
				return;

			// For. GroupEffect component 
			shared_ptr<GroupEffect> pGroupEffect = make_shared<GroupEffect>();
			pGroupEffectObj->Add_Component(pGroupEffect);
			pGroupEffectObj->Get_GroupEffect()->Set_Tag(pGroupEffectData->Get_GroupEffectDataTag());
			pGroupEffectObj->Get_GroupEffect()->Set_MemberEffectData(pGroupEffectData->Get_MemberEffectData());
			pGroupEffectObj->Get_GroupEffect()->Set_InitWorldMatrix(pGroupEffectObj->Get_Transform()->Get_WorldMatrix());
			pGroupEffectObj->Get_GroupEffect()->Set_MemberEffectMaterials();
			pGroupEffectObj->Set_Name(m_wstrEffectTag);
			pGroupEffectObj->Init();
			//TODO


			// For. Add Effect GameObject to current scene
			EVENTMGR.Create_Object(pGroupEffectObj);
			m_fHintEffectTimer = 0.f;
		}
	}
	else
	{
		EVENTMGR.Delete_Object(Get_Owner());
		return;
	}
}
