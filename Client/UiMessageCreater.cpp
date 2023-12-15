#include "pch.h"
#include "UiMessageCreater.h" 

#include "FontRenderer.h"
#include "MeshRenderer.h"
#include "UiMessageController.h"

UiMessageCreater::UiMessageCreater()
{
}

HRESULT UiMessageCreater::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    return S_OK;
}

void UiMessageCreater::Create_Message(wstring strMessage, shared_ptr<GameObject> pTarget)
{
    m_iType = 0;

    auto pScene = CUR_SCENE;
    vector<weak_ptr<GameObject>> addedObj;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Message_NotHere.dat", addedObj);

    if (nullptr != pTarget)
        Check_Distance(pTarget);

    _uint iSize = IDX(addedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = addedObj[i];
        if (true == pObj.expired())
            continue;

        wstring strName = pObj.lock()->Get_Name();
        if (L"UI_Message_NotHere_Bg" == strName)
        {
            auto pScript = make_shared<UiMessageController>(strMessage, pTarget, true, m_iType);
            pObj.lock()->Add_Component(pScript);
            pScript->Init();
        }
        else if (L"UI_Message_NotHere_Font" == strName)
        {
            auto pScript = make_shared<UiMessageController>(strMessage, pTarget, false, m_iType);
            pObj.lock()->Add_Component(pScript);
            pScript->Init();
        }
    }
}

void UiMessageCreater::Check_Distance(shared_ptr<GameObject> pTarget)
{
    _float3 pos = pTarget->GetOrAddTransform()->Get_State(Transform_State::POS).xyz();
    auto pPlayer = GET_PLAYER;

    if (pPlayer)
    {
        _float lengthSQ = (pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz() - pos).LengthSquared();

        if (lengthSQ > 12.f * 12.f)
            m_iType = 2;
        else if (lengthSQ > 6.f * 6.f)
            m_iType = 1;
    }
}
