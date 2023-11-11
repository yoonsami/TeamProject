#include "pch.h"
#include "BaseUI.h"

#include "Material.h"
#include "MeshRenderer.h"

BaseUI::BaseUI()
	:Component(COMPONENT_TYPE::BaseUI)
{
}

BaseUI::~BaseUI()
{
}

_bool BaseUI::Picked(POINT screenPos)
{
	if (PICK_TYPE::RECT == m_tagDesc.eType)
		return ::PtInRect(&m_tagDesc.rect, screenPos);
	else if (PICK_TYPE::CIRCLE == m_tagDesc.eType)
		return PtInCircle(screenPos);

	return false;
}

void BaseUI::Create(_float2 screenPos, _float2 size, shared_ptr<Material> material)
{
	auto Owner = Get_Owner();

	_float width = GRAPHICS.Get_ViewPort().Get_Width();
	_float height = GRAPHICS.Get_ViewPort().Get_Height();

	_float x = screenPos.x - width / 2.f;
	_float y = height / 2 - screenPos.y;

	_float3 pos = _float3(x, y, 0.f);

	Owner->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(pos, 1.f));
	Owner->GetOrAddTransform()->Scaled(_float3(size.x, size.y, 1.f));

	Owner->Set_LayerIndex(Layer_UI);

	if (Owner->Get_MeshRenderer() == nullptr)
		Owner->Add_Component(make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx")));

	Owner->Get_MeshRenderer()->Set_Material(material);

	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	Owner->Get_MeshRenderer()->Set_Mesh(mesh);
	Owner->Get_MeshRenderer()->Set_Pass(MeshRenderer::PASS_INFO::Default_UI);

	m_tagDesc.rect.left = LONG(screenPos.x - size.x / 2.f);
	m_tagDesc.rect.right = LONG(screenPos.x + size.x / 2.f);
	m_tagDesc.rect.top = LONG(screenPos.y - size.y / 2.f);
	m_tagDesc.rect.bottom = LONG(screenPos.y + size.y / 2.f);

}

void BaseUI::Create(PICK_TYPE eType, POINT ptPos1, POINT ptPos2)
{
	// Rect의 경우 두 점을 좌상, 우하로 사용해 저장
	// Circle의 경우 중점, 반지름을 구하는 다른 점으로 이용해서 센터와 반지름 저장

	m_tagDesc.eType = eType;

	if (PICK_TYPE::RECT == m_tagDesc.eType)
	{
		m_tagDesc.rect.left		= ptPos1.x;
		m_tagDesc.rect.top		= ptPos1.y;
		m_tagDesc.rect.right	= ptPos2.x;
		m_tagDesc.rect.bottom	= ptPos2.y;
	}
	else if (PICK_TYPE::CIRCLE == m_tagDesc.eType)
	{
		// 연산을 감안해서 루트는 생략
		// 후 거리 연산에서도 두점사이 계산시 마지막 루트 생략
		m_tagDesc.fValue = powf(static_cast<_float>(ptPos1.x) - static_cast<_float>(ptPos2.x), 2)
			+ powf(static_cast<_float>(ptPos1.y) - static_cast<_float>(ptPos2.y), 2);
		m_tagDesc.ptCenter = ptPos1;
	}

}

void BaseUI::AddOnClickedEvent(function<void(void)> func)
{
	_onClicked = func;
}

void BaseUI::InvokeOnClicked()
{
	if (_onClicked)
		_onClicked();
}

_bool BaseUI::PtInCircle(POINT screenPos)
{
	if (m_tagDesc.fValue > 
		powf(static_cast<_float>(m_tagDesc.ptCenter.x) - static_cast<_float>(screenPos.x), 2)
		+ powf(static_cast<_float>(m_tagDesc.ptCenter.y) - static_cast<_float>(screenPos.y), 2))
		return true;

	return false;
}
