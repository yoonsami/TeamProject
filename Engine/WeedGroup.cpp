#include "pch.h"
#include "WeedGroup.h"
#include "ModelRenderer.h"
#include "MeshRenderer.h"
#include "Camera.h"

WeedGroup::WeedGroup()
	:Component(COMPONENT_TYPE::WeedGroup)
{
}

WeedGroup::~WeedGroup()
{
}

HRESULT WeedGroup::Init()
{
	return S_OK;
}

void WeedGroup::Tick()
{
	//return;

	auto& frustum = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_Frustum();
	if (frustum.Contain_Sphere(m_pOwner.lock()->Get_CullPos(), m_pOwner.lock()->Get_CullRadius()) == false)
		return;

	m_fWindWeight += fDT * m_fWindWeightPM * m_fWindSpeed;
	if (m_fWindWeight >= 1.f)
	{
		m_fWindWeight = 1.f;
		m_fWindWeightPM = -1.f;
	}
	else if (m_fWindWeight <= 0.f)
	{
		m_fWindWeight = 0.f;
		m_fWindWeightPM = 1.f;
		m_Wind.x = Utils::Random_In_Range(-1.f, 1.f);
		m_Wind.y = Utils::Random_In_Range(-1.f, 1.f);
		m_Wind.z = Utils::Random_In_Range(-1.f, 1.f);
	}
}

void WeedGroup::Compute_CullPosHeight()
{
	// 최대최소계산
	for (auto& Weed : m_Weeds)
	{
		_float fHeight = Weed->Get_Transform()->Get_State(Transform_State::POS).y;
		
		Compute_MinMaxHeight(fHeight);
	}
	_float3 CullPos = m_pOwner.lock()->Get_CullPos();
	CullPos.y = (m_fHeightMax + m_fHeightMin) * 0.5f;
	m_pOwner.lock()->Set_CullPos(CullPos);
	_float AddRadius = fabs(m_fHeightMax) > fabs(m_fHeightMax) ? fabs(m_fHeightMax) : fabs(m_fHeightMin);
	_float CullRadius = m_pOwner.lock()->Get_CullRadius();
	CullRadius += AddRadius;
	m_pOwner.lock()->Set_CullRadius(CullRadius);
}

void WeedGroup::Compute_MinMaxHeight(_float _fHeight)
{
	if (m_fHeightMax < _fHeight)
		m_fHeightMax = _fHeight;
	if (m_fHeightMin > _fHeight)
		m_fHeightMin = _fHeight;
}




