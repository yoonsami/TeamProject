#include "pch.h"
#include "Hide_OrctongScript.h"
#include "Camera.h"
#include <MathUtils.h>

void Hide_OrctongScript::Tick()
{
	if (!KEYPUSH(KEY_TYPE::E))
		return;

	auto pPlayer = CUR_SCENE->Get_GameObject(L"Player");
	if (!pPlayer)
		return;

	_float3 vOwnerPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vPlayerPos = pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz();

	_float lengthSQ = (vOwnerPos - vPlayerPos).LengthSquared();

	if (lengthSQ >= 5.f * 5.f)
		return;

	auto camera = CUR_SCENE->Get_MainCamera()->Get_Camera();

	_float4x4 matView = camera->Get_ViewMat();
	_float4x4 matViewInv = matView.Invert();
	_float4x4 matProj = camera->Get_ProjMat();

	_float width = GRAPHICS.Get_ViewPort().Get_Width();
	_float height = GRAPHICS.Get_ViewPort().Get_Height();

	POINT MousePos;
	::GetCursorPos(&MousePos);
	::ScreenToClient(g_hWnd, &MousePos);
	_float2 screenPos{ (_float)MousePos.x, (_float)MousePos.y };

	_float viewX = (2.f * screenPos.x / width - 1.f) / matProj(0, 0);
	_float viewY = (-2.f * screenPos.y / height + 1.f) / matProj(1, 1);

	_float4 rayOriginPos = _float4(0, 0, 0, 1.f);
	_float4 rayDir = _float4(viewX, viewY, 1.f, 0.f);

	_float3 rayOriginPos_World = XMVector3TransformCoord(rayOriginPos, matViewInv);
	_float3 rayDir_World = XMVector3TransformNormal(rayDir, matViewInv);
	rayDir_World.Normalize();

	Ray ray = Ray(rayOriginPos_World, rayDir_World);
	_float dist = FLT_MAX;
	if (MathUtils::RayCast(Sphere3D{ Get_Owner()->Get_CullPos(),Get_Owner()->Get_CullRadius() }, Ray3D{ ray.position,ray.direction }, dist))
	{
		// Todo.
	}

}
