#pragma once
#include "Scene.h"

class ImGuiToolMgr;
class GranseedScene final : public Scene
{
public:
	GranseedScene();
	virtual ~GranseedScene() = default;

public:
	virtual void	Init() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;
	virtual void	Final_Tick() override;
	virtual void	Render() override;

	virtual HRESULT Load_Scene() override;

private:
	void			Load_DemoModel();
	void			Load_Camera();
	void			Load_Light();
};

