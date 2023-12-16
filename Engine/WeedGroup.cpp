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




