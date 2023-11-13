#include "pch.h"
#include "GroupEffectData.h"

#include "Utils.h"
#include "FileUtils.h"

GroupEffectData::GroupEffectData()
	:ResourceBase(ResourceType::GroupEffectData)
{

}

GroupEffectData::~GroupEffectData()
{
}

void GroupEffectData::Add_MemberEffect(MemberEffect_Desc tDesc)
{
	m_vMemberEffectData.push_back(tDesc);
}

void GroupEffectData::Load(const wstring& path)
{
	/* Load m_wstrTag and m_vMemberEffectData from file */

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(path, FileMode::Read);

	/* Tag */
	m_wstrTag = Utils::ToWString(file->Read<string>());
	_int iNumMembers = file->Read<_int>();

	/* Member Effects */
	for (_int i = 0; i < iNumMembers; i++)
	{
		MemberEffect_Desc tDesc;

		tDesc.wstrEffectTag = Utils::ToWString(file->Read<string>());
		tDesc.eType = EFFECT_TYPE(file->Read<_int>());
		tDesc.fCreateTime = file->Read<_float>();

		tDesc.bIsActive = file->Read<_bool>();

		tDesc.vPivot_Pos = file->Read<_float3>();
		tDesc.vPivot_Scale = file->Read<_float3>();
		tDesc.vPivot_Rotation = file->Read<_float3>();

		m_vMemberEffectData.push_back(tDesc);
	}
}
