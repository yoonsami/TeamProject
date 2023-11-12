#include "pch.h"
#include "GroupEffect.h"

#include "Utils.h"
#include "FileUtils.h"
#include "MeshEffect.h"
#include "MeshEffectData.h"

GroupEffect::GroupEffect()
	: Component(COMPONENT_TYPE::GroupEffect)
{
}

GroupEffect::~GroupEffect()
{
}

void GroupEffect::Init(void* pArg)
{
}

void GroupEffect::Tick()
{
	_int iIndex = 0;
	for(auto& iter : m_vMemberEffectData)
	{
		if ( !iter.bIsActive && iter.fCreateTime < m_fCurrAge)
		{
			if (GroupEffectData::TYPE_MESHEFFECT == iter.eType)
				Create_MeshEffect(iIndex);
			else if (GroupEffectData::TYPE_PARTICLE == iter.eType)
				Create_Particle(iIndex);
			
			iter.bIsActive = true;
		}
		iIndex++;
	}
}

void GroupEffect::Final_Tick()
{
	if (m_bIsStop)
		return;

	m_fCurrAge += fDT;

	// For. Check all effect finished
	_bool bIsAllActive = true;
	for (auto& iter : m_vMemberEffectData)
	{
		if (!iter.bIsActive)
			bIsAllActive = false;
	}

	if (bIsAllActive)
	{
		_bool bIsAllNullptr = true;
		for (auto& iter : m_vMemberEffects)
		{
			if (nullptr == iter)
				bIsAllNullptr = false;
		}

		if (bIsAllNullptr)
		{
			if (m_bIsLoop)
			{
				m_vMemberEffects.clear();
				m_fCurrAge = 0.f;
			}
			else
			{
				CUR_SCENE->Remove_GameObject(Get_Owner());
				return;
			}
		}
	}
}

void GroupEffect::Save(const wstring& path)
{
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(path, FileMode::Write);

	/* Tag */
	file->Write<string>(Utils::ToString(m_wstrTag));
	file->Write<_int>((_int)m_vMemberEffectData.size());

	/* Member Effect Data */
	for (auto& iter : m_vMemberEffectData)
	{
		file->Write<string>(Utils::ToString(iter.wstrEffectTag));
		file->Write<_int>(iter.eType);
		file->Write<_float>(iter.fCreateTime);

		file->Write<_bool>(false);

		file->Write<_float3>(iter.vPivot_Pos);
		file->Write<_float3>(iter.vPivot_Scale);
		file->Write<_float3>(iter.vPivot_Rotation);
	}
}

void GroupEffect::Set_Stop(_bool bIsStop)
{
	//m_bIsStop = bIsStop;

	//for (auto& iter : m_vMemberEffects)
	//	iter->Get_MeshEffect()->Set_IsAlwaysShowFirstTick(bIsStop);
}

void GroupEffect::Create_MeshEffect(_int iIndex)
{
	auto iter = m_vMemberEffectData[iIndex];
	
	shared_ptr<GameObject> EffectObj = make_shared<GameObject>();

	// For. Transform 
	EffectObj->GetOrAddTransform();
	EffectObj->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + _float4(iter.vPivot_Pos, 0.f));
	EffectObj->Get_Transform()->Scaled(iter.vPivot_Scale);
	EffectObj->Get_Transform()->Set_Rotation(iter.vPivot_Rotation);

	// For. Shader 
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");

	// For.  Mesh Effect  
	shared_ptr<MeshEffect> meshEffect = make_shared<MeshEffect>(shader);
	EffectObj->Add_Component(meshEffect);
	wstring wstrMeshEffectDataKey = iter.wstrEffectTag;
	Utils::DetachExt(wstrMeshEffectDataKey);
	shared_ptr<MeshEffectData> meshEffectData = RESOURCES.Get<MeshEffectData>(wstrMeshEffectDataKey);
	MeshEffectData::DESC tDesc = meshEffectData->Get_Desc();
	EffectObj->Get_MeshEffect()->Init(&tDesc);
	EffectObj->Get_MeshEffect()->Set_IsImmortal(false);
	EffectObj->Get_MeshEffect()->Set_IsAlwaysShowFirstTick(m_bIsStop);

	// For. Add to vector 
	m_vMemberEffects.push_back(EffectObj);
	
	// For. Add to scene
	CUR_SCENE->Add_GameObject(EffectObj);
}

void GroupEffect::Create_Particle(_int iIndex)
{

}
