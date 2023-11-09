#include "pch.h"
#include "UiLoadingScript.h"

#include "MeshRenderer.h"
#include "Material.h"

UiLoadingScript::UiLoadingScript(_bool bIsEven)
    : m_bIsEven(bIsEven)
{
}

HRESULT UiLoadingScript::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_strEvenTextureName.push_back(L"UI_Loading0");
    m_strEvenTextureName.push_back(L"UI_Loading2");
    m_strEvenTextureName.push_back(L"UI_Loading4");
    m_strEvenTextureName.push_back(L"UI_Loading6");
    m_strEvenTextureName.push_back(L"UI_Loading8");
    m_strEvenTextureName.push_back(L"UI_Loading10");
    m_strEvenTextureName.push_back(L"UI_Loading12");

    m_strOddTextureName.push_back(L"UI_Loading1");
    m_strOddTextureName.push_back(L"UI_Loading3");
    m_strOddTextureName.push_back(L"UI_Loading5");
    m_strOddTextureName.push_back(L"UI_Loading7");
    m_strOddTextureName.push_back(L"UI_Loading9");
    m_strOddTextureName.push_back(L"UI_Loading11");
    m_strOddTextureName.push_back(L"UI_Loading13");


    if(true == m_bIsEven)
    {
        m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_strEvenTextureName[m_iIndex]), TextureMapType::DIFFUSE);
    }
    else
    {
        m_eType = CHANGE_TYPE::OFF_IDLE;
        m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_strOddTextureName[m_iIndex]), TextureMapType::DIFFUSE);
    }




    return S_OK;
}

void UiLoadingScript::Tick()
{
	if (m_pOwner.expired())
		return;

    m_fCheckTime += fDT;

    if(true == m_bIsEven)
    {
        switch (m_eType)
        {
        case CHANGE_TYPE::ON_IDLE:
            On_Idle();
            break;

        case CHANGE_TYPE::OFF:
            Off();
            break;

        case CHANGE_TYPE::CHANGE:
            Change();
            break;

        case CHANGE_TYPE::OFF_IDLE:
            Off_Idle();
            break;

        case CHANGE_TYPE::ON:
            On();
            break;
        }
    }
}

void UiLoadingScript::Change_Texture()
{
    ++m_iIndex;
    if (m_strOddTextureName.size() == m_iIndex)
        m_iIndex = 0;

    m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_strOddTextureName[m_iIndex]), TextureMapType::DIFFUSE);
}

void UiLoadingScript::On_Idle()
{
    if (m_fMaxIdleTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eType = CHANGE_TYPE::OFF;
    }
}

void UiLoadingScript::Off()
{
    if (m_fOnOffTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eType = CHANGE_TYPE::CHANGE;
        m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;

        return;
    }

    _float fRatio = 1.f - (m_fCheckTime / m_fOnOffTime);

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = fRatio;
}

void UiLoadingScript::Change()
{
    m_fCheckTime = 0.f;
    m_eType = CHANGE_TYPE::OFF_IDLE;

    if (true == m_bIsEven)
    {
        ++m_iIndex;
        if (m_strEvenTextureName.size() == m_iIndex)
            m_iIndex = 0;

        m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_strEvenTextureName[m_iIndex]), TextureMapType::DIFFUSE);
    }
    else
    {
        ++m_iIndex;
        if (m_strOddTextureName.size() == m_iIndex)
            m_iIndex = 0;

        m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_strOddTextureName[m_iIndex]), TextureMapType::DIFFUSE);
    }

}

void UiLoadingScript::Off_Idle()
{
    if (m_fMaxIdleTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eType = CHANGE_TYPE::ON;
    }
}

void UiLoadingScript::On()
{
    if (m_fOnOffTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eType = CHANGE_TYPE::ON_IDLE;
        m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;

        CUR_SCENE->Get_GameObject(L"UI_Loading_Odd")->Get_Script<UiLoadingScript>()->Change_Texture();

        return;
    }

    _float fRatio = m_fCheckTime / m_fOnOffTime;

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = fRatio;
}
