#include "pch.h"
#include "DragonBallLightning_Script.h"

#include "GroupEffect.h"

DragonBallLightning_Script::DragonBallLightning_Script()
{
}

DragonBallLightning_Script::~DragonBallLightning_Script()
{

}

void DragonBallLightning_Script::Tick()
{
	m_fCurrAge += fDT;
	m_fCoolTime_CreateEffect += fDT;

	// Check Owner Dead
	if (m_fCurrAge > 1.f)
	{
		EVENTMGR.Delete_Object(Get_Owner());
		return;
	}

	// Create Effect
	
	if (m_fCoolTime_CreateEffect > 0.1f)
	{
		_int iCreateCnt = rand() % 20 + 5;
		for (_int i = 0; i < iCreateCnt; i++)
		{
			// Set Position 
			_float3 vPos = { 0.f, 0.f, 0.f };
			_float fRandRadius = rand() % 30 + 1;
			_float fRandAngle = 0.f;
			if (RIGHT == m_eDirection)
				fRandAngle = rand() % 180 + 270;
			else if (LEFT == m_eDirection)
				fRandAngle = rand() % 180 + 90;
			fRandAngle *= (XM_PI / 180.f);
			vPos.x = fRandRadius * cos(fRandAngle);
			vPos.z = fRandRadius * sin(fRandAngle);

			Add_Effect(L"DragonBall_Lightning", _float4x4::CreateTranslation(vPos));
		}
		
		m_fCoolTime_CreateEffect = 0.f;
	}
}

void DragonBallLightning_Script::Add_Effect(const wstring& wstrSkillTag, const _float4x4& matWorldPos)
{
	shared_ptr<GameObject> pGroupEffectObj = make_shared<GameObject>();

	// For. Transform 
	pGroupEffectObj->GetOrAddTransform();
	pGroupEffectObj->Get_Transform()->Set_WorldMat(matWorldPos);

	// For. GroupEffectData 
	wstring wstrFileName = wstrSkillTag + L".dat";
	wstring wtsrFilePath = TEXT("..\\Resources\\EffectData\\GroupEffectData\\") + wstrFileName;
	shared_ptr<GroupEffectData> pGroupEffectData = RESOURCES.GetOrAddGroupEffectData(wstrSkillTag, wtsrFilePath);

	if (pGroupEffectData == nullptr)
		return;

	// For. GroupEffect component 
	shared_ptr<GroupEffect> pGroupEffect = make_shared<GroupEffect>();

	pGroupEffectObj->Add_Component(pGroupEffect);
	pGroupEffectObj->Get_GroupEffect()->Set_Tag(pGroupEffectData->Get_GroupEffectDataTag());
	pGroupEffectObj->Get_GroupEffect()->Set_MemberEffectData(pGroupEffectData->Get_MemberEffectData());
	pGroupEffectObj->Get_GroupEffect()->Set_InitWorldMatrix(pGroupEffectObj->Get_Transform()->Get_WorldMatrix());
	pGroupEffectObj->Get_GroupEffect()->Set_MemberEffectMaterials();
	pGroupEffectObj->Set_Name(wstrSkillTag);
	pGroupEffectObj->Init();

	// For. Add Effect GameObject to current scene
	EVENTMGR.Create_Object(pGroupEffectObj);
}
