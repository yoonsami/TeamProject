#pragma once
#include "Scene.h"
class LogoScene :
    public Scene
{
public:
	LogoScene();
	~LogoScene();

public:
	virtual void Init() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Final_Tick() override;

	virtual HRESULT Load_Scene() override;

private:

	void Load_Camera();
	void Load_Ui();
};

