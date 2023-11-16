#include "pch.h"
#include "UiMonsterHp.h" 

#include "Camera.h"
#include "MeshRenderer.h"

UiMonsterHp::UiMonsterHp()
{
}

HRESULT UiMonsterHp::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    auto pScene = CUR_SCENE;
    m_pFrontHp = pScene->Get_UI(L"UI_MonsterHp0");
    m_pBackHp = pScene->Get_UI(L"UI_MonsterHp1");
    m_pBgHp = pScene->Get_UI(L"UI_MonsterHp2");
    m_pCamera = pScene->Get_Camera(L"Default");

    return S_OK;
}

void UiMonsterHp::Tick()
{
	if (true == m_pOwner.expired()      ||
        true == m_pFrontHp.expired()    ||
        true == m_pBackHp.expired()     ||
        true == m_pCamera.expired()     ||
        true == m_pBgHp.expired())
		return;

    Check_Target();
    Change_Hp_Ratio();
    Change_Hp_Slow();
    Update_Target_Pos();

    if (KEYTAP(KEY_TYPE::F5))
        m_pOwner.lock()->Get_Hurt(10.f);
}

void UiMonsterHp::Check_Target()
{
    if (m_pOwner.expired())
        return;

    if (0.f >= m_pOwner.lock()->Get_CurHp())
    {
        auto pScene = CUR_SCENE;
        pScene->Remove_GameObject(m_pOwner.lock());
        pScene->Remove_GameObject(m_pFrontHp.lock());
        pScene->Remove_GameObject(m_pBackHp.lock());
        pScene->Remove_GameObject(m_pBgHp.lock());
    }
}

void UiMonsterHp::Change_Hp_Ratio()
{
    _float HpRatio = m_pOwner.lock()->Get_HpRatio();
    m_pFrontHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = HpRatio;

    m_fRatio = m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0];

    if (HpRatio != m_fRatio && false == m_bIsWork)
    {
        m_bIsWork = true;
        m_fSpeed = m_fRatio - HpRatio;
        m_fTargetRatio = HpRatio;
    }
}

void UiMonsterHp::Change_Hp_Slow()
{
    if (false == m_bIsWork)
        return;

    m_fRatio -= m_fSpeed * fDT;
    if (m_fRatio < m_fTargetRatio)
    {
        m_fRatio = m_fTargetRatio;
        m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_fTargetRatio;
        m_bIsWork = false;
    }
    else
    {
        m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_fRatio;
    }
}

void UiMonsterHp::Update_Target_Pos()
{
    _float4 vecPos = m_pOwner.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.y = 3.f;
    m_pFrontHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[1] = vecPos;
    m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[1] = vecPos;
    m_pBgHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[1] = vecPos;

    _float4x4 matWorld = m_pCamera.lock()->GetOrAddTransform()->Get_WorldMatrix();
    _float4x4 matView = m_pCamera.lock()->Get_Camera()->Get_ViewMat();
    _float4x4 matProj = m_pCamera.lock()->Get_Camera()->Get_ProjMat();

    m_pFrontHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[0] = matView;
    m_pFrontHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[1] = matProj;

    m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[0] = matView;
    m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[1] = matProj;

    m_pBgHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[0] = matView;
    m_pBgHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[1] = matProj;
}
