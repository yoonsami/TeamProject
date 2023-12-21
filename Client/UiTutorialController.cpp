#include "pch.h"
#include "UiTutorialController.h" 

#include "MeshRenderer.h"

UiTutorialController::UiTutorialController()
{
}

HRESULT UiTutorialController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    m_iIndex = 1;

    auto& pResource = RESOURCES;
    m_pTexture.resize(23);
    for (_uint i = 0; i < 23; ++i)
    {
        wstring strKey = L"UI_Tutorial_R_" + to_wstring(i);
        m_pTexture[i] = pResource.Get<Texture>(strKey);
    }
    
    vector<weak_ptr<GameObject>> addedObj;
    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Tutorial.dat", addedObj);

    m_pImage = addedObj[0];


    m_pImage.lock()->Set_Render(false);

    return S_OK;
}

void UiTutorialController::Tick()
{
	if (m_pOwner.expired())
		return;

    if (true == m_bIsStart)
        Change_Texture();
}

void UiTutorialController::Start_Tutorial()
{
    m_bIsStart = true;
    m_pImage.lock()->Set_Render(true);
}

void UiTutorialController::Change_Texture()
{


    if (KEYTAP(KEY_TYPE::LBUTTON))
    {
        if (23 == m_iIndex)
        {
            Finish_Tutorial();
        }

        else if (true == m_bIsEnd)
        {
            m_bIsEnd = false;

            End_Tutorial();
        }

        else if (false == m_pImage.expired() && false == m_pTexture[m_iIndex].expired())
        {
            m_pImage.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(m_pTexture[m_iIndex++].lock(), TextureMapType::DIFFUSE);

            if (4 == m_iIndex || 7 == m_iIndex || 11 == m_iIndex)
            {
                m_bIsEnd = true;
            }
        }
    }
}

void UiTutorialController::Finish_Tutorial()
{
    m_bIsStart = false;

    EVENTMGR.Delete_Object(m_pImage.lock());
    EVENTMGR.Delete_Object(m_pOwner.lock());
}

void UiTutorialController::End_Tutorial()
{
    m_bIsStart = false;
    m_pImage.lock()->Set_Render(false);

    m_pImage.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(m_pTexture[m_iIndex++].lock(), TextureMapType::DIFFUSE);
}
