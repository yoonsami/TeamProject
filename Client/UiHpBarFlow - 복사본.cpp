#include "pch.h"
#include "UiCharChange.h"

#include "MeshRenderer.h"

UiCharChange::UiCharChange()
{
}

HRESULT UiCharChange::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_vecDesc.resize(3);
    m_vecDesc[0].fCoolTime = 8.f;
    m_vecDesc[1].fCoolTime = 8.f;
    m_vecDesc[2].fCoolTime = 8.f;

    m_vecDesc[0].fAccTime = m_vecDesc[0].fCoolTime;
    m_vecDesc[1].fAccTime = m_vecDesc[1].fCoolTime;
    m_vecDesc[2].fAccTime = m_vecDesc[2].fCoolTime;

    m_pObj.resize(3);
    m_pObj[0] = CUR_SCENE->Get_GameObject(L"UI_Char_Change0");
    m_pObj[1] = CUR_SCENE->Get_GameObject(L"UI_Char_Change1");
    m_pObj[2] = CUR_SCENE->Get_GameObject(L"UI_Char_Change2");



    return S_OK;
}

void UiCharChange::Tick()
{
	if (m_pOwner.expired())
		return;

    Check_Change_Cool();
}

void UiCharChange::Set_Hero(_uint iIndex, HERO eHero)
{
    if (m_pOwner.expired() || 2 < iIndex)
        return;


}

_bool UiCharChange::IsChangePossible(_uint iIndex)
{
    if (m_pOwner.expired() || 2 < iIndex)
        return;


}

void UiCharChange::Check_Change_Cool()
{
    _uint iSize = IDX(m_vecDesc.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& Desc = m_vecDesc[i];
        if (HERO::MAX == Desc.eHero)
        {
            Desc.fAccTime = Desc.fCoolTime + 1.f;
        }
        else
        {
            if (Desc.fCoolTime > Desc.fAccTime)
                Desc.fAccTime += fDT;
        }
    }
}

void UiCharChange::Set_Param_Value()
{
    _uint iSize = IDX(m_vecDesc.size());
    for (_uint i = 0; i < iSize; ++i)
        if (true == m_pObj[i].expired())
            return;

    for (_uint i = 0; i < iSize; ++i)
    {
        m_pObj[i].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_vecDesc[i].fAccTime / m_vecDesc[i].fCoolTime;
    }

}
