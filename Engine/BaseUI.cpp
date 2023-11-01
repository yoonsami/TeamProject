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
	return ::PtInRect(&_rect, screenPos);
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

	_rect.left = LONG(screenPos.x - size.x / 2.f);
	_rect.right = LONG(screenPos.x + size.x / 2.f);
	_rect.top = LONG(screenPos.y - size.y / 2.f);
	_rect.bottom = LONG(screenPos.y + size.y / 2.f);

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
