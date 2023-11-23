#include "pch.h"
#include "GroupEffectOwner.h"

#include "GroupEffect.h"

GroupEffectOwner::GroupEffectOwner()
	: Component(COMPONENT_TYPE::GroupEffectOwner)
{
}

GroupEffectOwner::~GroupEffectOwner()
{
}

void GroupEffectOwner::Tick()
{
	if (m_bIsFirstTick)
	{
		PlayEffect();
		m_bIsFirstTick = false;
	}

	if (m_pGroupEffect.expired())
	{
		CUR_SCENE->Remove_GameObject(Get_Owner());
		return;
	}
}

void GroupEffectOwner::PlayEffect()
{
	shared_ptr<GameObject> pGroupEffectObj = make_shared<GameObject>();

	// For. Transform 
	pGroupEffectObj->GetOrAddTransform();
	pGroupEffectObj->Get_Transform()->Set_State(Transform_State::POS, m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS));
	pGroupEffectObj->Get_Transform()->Set_Quaternion(Get_Transform()->Get_Rotation());

	// For. GroupEffectData 
	wstring wstrFileName = m_wstrGroupEffectTag + L".dat";
	wstring wtsrFilePath = TEXT("..\\Resources\\EffectData\\GroupEffectData\\") + wstrFileName;
	shared_ptr<GroupEffectData> pGroupEffectData = RESOURCES.GetOrAddGroupEffectData(m_wstrGroupEffectTag, wtsrFilePath);

	if (pGroupEffectData == nullptr)
		return;

	// For. GroupEffect component 
	shared_ptr<GroupEffect> pGroupEffect = make_shared<GroupEffect>();
	pGroupEffectObj->Add_Component(pGroupEffect);
	pGroupEffectObj->Get_GroupEffect()->Set_Tag(pGroupEffectData->Get_GroupEffectDataTag());
	pGroupEffectObj->Get_GroupEffect()->Set_MemberEffectData(pGroupEffectData->Get_MemberEffectData());
	pGroupEffectObj->Get_GroupEffect()->Set_InitWorldMatrix(Get_Transform()->Get_WorldMatrix());
	pGroupEffectObj->Set_Name(m_wstrGroupEffectTag);
	m_pGroupEffect = pGroupEffectObj;

	// For. Add Effect GameObject to current scene
	CUR_SCENE->Add_GameObject(m_pGroupEffect.lock());
}

