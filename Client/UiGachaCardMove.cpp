#include "pch.h"
#include "UiGachaCardMove.h" 

#include "Utils.h"
#include "MeshRenderer.h"

UiGachaCardMove::UiGachaCardMove(_uint iIndex)
    : m_iIndex(iIndex)
{
}

HRESULT UiGachaCardMove::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

	m_fMaxTime = 0.5f;
	m_fCheckTime = 0.f;

	m_fOriginScale = { 272.f, 444.f, 0.f };
	m_fCurScale = m_fOriginScale;
	m_fScaleChangeValue = m_fOriginScale.x;
	m_fTrunPerTime = 2.1f;

	switch (m_iIndex)
	{
	case 0:
		m_vecFirstPos = _float4(80.f, -120.f, 5.f, 1.f);
		m_vecArrivalPos = _float4(-600.f, 230.f, 5.f, 1.f);
		break;
	case 1:
		m_vecFirstPos = _float4(60.f, -90.f, 5.1f, 1.f);
		m_vecArrivalPos = _float4(-300.f, 230.f, 5.1f, 1.f);
		break;

	case 2:
		m_vecFirstPos = _float4(40.f, -60.f, 5.2f, 1.f);
		m_vecArrivalPos = _float4(0.f, 230.f, 5.2f, 1.f);
		break;

	case 3:
		m_vecFirstPos = _float4(20.f, -30.f, 5.3f, 1.f);
		m_vecArrivalPos = _float4(300.f, 230.f, 5.3f, 1.f);
		break;

	case 4:
		m_vecFirstPos = _float4(0.f, 0.f, 5.4f, 1.f);
		m_vecArrivalPos = _float4(600.f, 230.f, 5.4f, 1.f);
		break;

	case 5:
		m_vecFirstPos = _float4(-20.f, 30.f, 5.5f, 1.f);
		m_vecArrivalPos = _float4(-600.f, -250.f, 5.5f, 1.f);
		break;

	case 6:
		m_vecFirstPos = _float4(-40.f, 60.f, 5.6f, 1.f);
		m_vecArrivalPos = _float4(-300.f, -250.f, 5.6f, 1.f);
		break;

	case 7:
		m_vecFirstPos = _float4(-60.f, 90.f, 5.7f, 1.f);
		m_vecArrivalPos = _float4(0.f, -250.f, 5.7f, 1.f);
		break;

	case 8:
		m_vecFirstPos = _float4(-80.f, 120.f, 5.8f, 1.f);
		m_vecArrivalPos = _float4(300.f, -250.f, 5.8f, 1.f);
		break;

	case 9:
		m_vecFirstPos = _float4(-100.f, 150.f, 5.9f, 1.f);
		m_vecArrivalPos = _float4(600.f, -250.f, 5.9f, 1.f);
		break;
	}
	m_vecDir = { m_vecArrivalPos.x - m_vecFirstPos.x,  m_vecArrivalPos.y - m_vecFirstPos.y };

	_float fRand = Utils::Random_In_Range(0.f, 1.f);
	if (0.01f > fRand)
	{
		m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"UI_Gacha_Card_Back0"), TextureMapType::DIFFUSE);
		m_strTextureTag = L"Big_Card_Ace_Line";
	}
	else if (0.02f > fRand)
	{
		m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"UI_Gacha_Card_Back1"), TextureMapType::DIFFUSE);
		m_strTextureTag = L"Big_Card_Dellons_Line";
	}
	else if (0.03f > fRand)
	{
		m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"UI_Gacha_Card_Back3"), TextureMapType::DIFFUSE);
		m_strTextureTag = L"Big_Card_Kyle_Line";
	}
	else if (0.04f > fRand)
	{
		m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"UI_Gacha_Card_Back4"), TextureMapType::DIFFUSE);
		m_strTextureTag = L"Big_Card_Yeopo_Line";
	}
	else if (0.4f > fRand)
	{
		m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"UI_Gacha_Card_Back2"), TextureMapType::DIFFUSE);
	}
	else
	{
		m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"UI_Gacha_Card_Back5"), TextureMapType::DIFFUSE);
	}

	// 오픈할 카드 정해야함
	// 우선 test code


    return S_OK;
}

void UiGachaCardMove::Tick()
{
	if (m_pOwner.expired())
		return;

	switch (m_eState)
	{
	case STATE::START:
		Move();
		break;
	case STATE::IDLE:
		Idle();
		break;
	case STATE::OPEN:
		Open();
		break;
	}
}

void UiGachaCardMove::Card_Open()
{
	m_eState = STATE::OPEN;
	m_fCheckTime = 0.f;
}

void UiGachaCardMove::Move()
{
	m_fCheckTime += fDT;
	if (m_fMaxTime < m_fCheckTime)
	{
		m_fCheckTime = 0.f;
		m_eState = STATE::IDLE;
		
		return;
	}
	
	auto pTransform = m_pOwner.lock()->GetOrAddTransform();
	_float4 vecPos = pTransform->Get_State(Transform_State::POS);
	vecPos.x += m_vecDir.x * fDT / m_fMaxTime;
	vecPos.y += m_vecDir.y * fDT / m_fMaxTime;

	pTransform->Set_State(Transform_State::POS, vecPos);

	// pTransform->Turn(_float3(0.f, 1.f, 0.f), m_fTurnSpeed);
	switch (m_eChangeType)
	{
	case UiGachaCardMove::CHANGE_TYPE::DOWN:
		m_fCurScale.x -= m_fScaleChangeValue * fDT * m_fTrunPerTime / m_fMaxTime;
		break;
	case UiGachaCardMove::CHANGE_TYPE::UP:
		m_fCurScale.x += m_fScaleChangeValue * fDT * m_fTrunPerTime / m_fMaxTime;
		break;
	}

	if (0.01f > m_fCurScale.x)
	{
		m_fCurScale.x = 0.01f;
		m_eChangeType = CHANGE_TYPE::UP;
	}
	else if (272.f < m_fCurScale.x)
	{
		m_fCurScale.x = 272.f;
		m_eChangeType = CHANGE_TYPE::DOWN;
	}

	m_pOwner.lock()->GetOrAddTransform()->Scaled(m_fCurScale);
}

void UiGachaCardMove::Idle()
{
	// 쉐이더 코드

}

void UiGachaCardMove::Open()
{
	switch (m_eChangeType)
	{
	case UiGachaCardMove::CHANGE_TYPE::DOWN:
		m_fCurScale.x -= m_fScaleChangeValue * fDT * m_fTrunPerTime / m_fMaxTime;
		break;
	case UiGachaCardMove::CHANGE_TYPE::UP:
		m_fCurScale.x += m_fScaleChangeValue * fDT * m_fTrunPerTime / m_fMaxTime;
		break;
	}

	if (0.01f > m_fCurScale.x)
	{
		m_fCurScale.x = 0.01f;
		m_eChangeType = CHANGE_TYPE::UP;
		if(0 != m_strTextureTag.length())
			m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_strTextureTag), TextureMapType::DIFFUSE);
	}
	else if (272.f < m_fCurScale.x)
	{
		m_fCurScale.x = 272.f;
		m_eState = STATE::IDLE;
	}

	m_pOwner.lock()->GetOrAddTransform()->Scaled(m_fCurScale);
}
