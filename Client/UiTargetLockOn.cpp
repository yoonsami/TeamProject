#include "pch.h"
#include "UiTargetLockOn.h" 

#include "Camera.h"
#include "MeshRenderer.h"

UiTargetLockOn::UiTargetLockOn()
{
}

HRESULT UiTargetLockOn::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_pCamera = CUR_SCENE->Get_Camera(L"Default");
    m_pLockOn1 = CUR_SCENE->Get_UI(L"UI_Target_LockOn1");

    m_fOriginScale = m_vecScale.x = m_pLockOn1.lock()->GetOrAddTransform()->Get_Scale().x;
    m_vecScale.y = m_pLockOn1.lock()->GetOrAddTransform()->Get_Scale().y;

    m_fMaxScale = 100.f;
    m_fSpeed = m_fMaxScale - m_fOriginScale + 10.f;

    return S_OK;
}

void UiTargetLockOn::Tick()
{
	if (m_pOwner.expired())
		return;

    Check_Target();
    Set_Owner_Render();
    Change_Scale();
}

void UiTargetLockOn::Late_Tick()
{
    if (m_pOwner.expired())
        return;

    Update_Target_Pos();
}

void UiTargetLockOn::Set_Target(shared_ptr<GameObject> pObj)
{
    if (nullptr != pObj)
        m_pTarget = pObj;
    else
        m_pTarget.reset();
}

void UiTargetLockOn::Change_Scale()
{
    if (STATE::UP == m_eState)
    {
        m_vecScale.x = m_vecScale.y += m_fSpeed * fDT;

        if (m_vecScale.x > m_fMaxScale)
        {
            m_eState = STATE::DOWN;
        }
    }
    else if (STATE::DOWN == m_eState)
    {
        m_vecScale.x = m_vecScale.y -= m_fSpeed * fDT;

        if (m_vecScale.x < m_fOriginScale)
        {
            m_eState = STATE::UP;
        }
    }

    m_pLockOn1.lock()->GetOrAddTransform()->Scaled(m_vecScale);
}

void UiTargetLockOn::Update_Target_Pos()
{
    if (true == m_pTarget.expired() || true == m_pLockOn1.expired() || true == m_pCamera.expired())
        return;

    _float4 vecPos = m_pTarget.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.y = 1.f;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[1] = vecPos;
    m_pLockOn1.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[1] = vecPos;
    
    _float4x4 matWorld = m_pCamera.lock()->GetOrAddTransform()->Get_WorldMatrix();
    _float4x4 matView = m_pCamera.lock()->Get_Camera()->Get_ViewMat();
    _float4x4 matProj = m_pCamera.lock()->Get_Camera()->Get_ProjMat();

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[0] = matView;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[1] = matProj;

    m_pLockOn1.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[0] = matView;
    m_pLockOn1.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[1] = matProj;
}

void UiTargetLockOn::Set_Owner_Render()
{
    _bool bRender = true;
    if (true == m_pTarget.expired())
        bRender = false;

    if (m_bIsRender != bRender)
    {
        m_bIsRender = bRender;
        m_pOwner.lock()->Set_Render(m_bIsRender);
        m_pLockOn1.lock()->Set_Render(m_bIsRender);
    }
}

void UiTargetLockOn::Check_Target()
{
    if (true == m_pTarget.expired() || true == m_pLockOn1.expired())
        return;

    if (0 >= m_pTarget.lock()->Get_CurHp())
        m_pTarget.reset();
}
