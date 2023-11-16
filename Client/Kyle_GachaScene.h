#pragma once
#include "Scene.h"

struct GachaSceneDesc
{
	wstring strMapFileName;
	wstring strCharModelNmae;
	HERO eHeroType;
};

class Kyle_GachaScene :
    public Scene
{
public:
	Kyle_GachaScene(const GachaSceneDesc& desc);
	~Kyle_GachaScene();

public:
	virtual void Init() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Final_Tick() override;

	virtual HRESULT Load_Scene() override;

private:
	void Load_SkyBox();
	void Load_Player();

	void Load_Camera();

	void Load_Light();

private:
	GachaSceneDesc m_Desc{};

};