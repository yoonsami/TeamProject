#include "pch.h"
#include "UiMarkNpc.h" 

#include "Camera.h"

UiMarkNpc::UiMarkNpc(NPCTYPE eType)
    : m_eType(eType)
{
}

HRESULT UiMarkNpc::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

	m_pCamera = CUR_SCENE->Get_Camera(L"Default");

	wstring strTextureTag;
	switch (m_eType)
	{
	case NPCTYPE::QUEST:
		strTextureTag = L"UI_Mark_Quest";
		break;
	case NPCTYPE::GACHA:
		strTextureTag = L"UI_Mark_Gacha";
		break;
	case NPCTYPE::MAX:
		return E_FAIL;
	}



    return S_OK;
}

void UiMarkNpc::Tick()
{
	if (true == m_pOwner.expired() ||
		true == m_pCamera.expired())
		return;

	Check_In_Screen();

}

void UiMarkNpc::Check_In_Screen()
{
	_float3 cullPos = m_pOwner.lock()->GetOrAddTransform()->Get_State(Transform_State::POS).xyz();
	_float cullRadius = 1.5f;
	Frustum frustum = m_pCamera.lock()->Get_Camera()->Get_Frustum();

	if (frustum.Contain_Sphere(cullPos, cullRadius))
		m_pOwner.lock()->Set_Render(true);
	else
		m_pOwner.lock()->Set_Render(false);
}
