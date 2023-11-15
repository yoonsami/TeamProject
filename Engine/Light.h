#pragma once
#include "Component.h"

enum class LIGHT_TYPE : _uchar
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void Tick() override;

	virtual void Final_Tick() override;

	void Render();
	void Render_Shadow(shared_ptr<Scene> scene);

public:
	LightInfo& Get_LightInfo() { return m_LightInfo; }
	void Set_Color(const LightColor& color) { m_LightInfo.color = color; }
	void Set_Ambient(const Color& color) { m_LightInfo.color.ambient = color; }
	void Set_Diffuse(const Color& color) { m_LightInfo.color.diffuse = color; }
	void Set_Specular(const Color& color) { m_LightInfo.color.specular = color; }
	void Set_Emissive(const Color& color) { m_LightInfo.color.emissive = color; }
	void Set_LightDir(const _float3& dir) { m_LightInfo.vDirection = dir; }

	void Set_LightType(LIGHT_TYPE type);
	void Set_LightRange(_float range) { m_LightInfo.range = range; }
	void Set_LightAngle(_float angle) { m_LightInfo.angle = angle; }

	void Set_LightIndex(_char index) { m_iLightIndex = index; }
	shared_ptr<GameObject> Get_ShadowCamera() { return m_pShadowCamera; }

private:
	LightInfo m_LightInfo{};
	_char m_iLightIndex = -1;

	shared_ptr<Mesh> m_pVolumeMesh;
	shared_ptr<Material> m_pLightMaterial;
	shared_ptr<GameObject> m_pShadowCamera;

};

