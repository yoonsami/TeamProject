#pragma once
#include "Scene.h"

class Loader;
class LoadingScene final : public Scene
{
public:
	LoadingScene(shared_ptr<Scene> nextScene);
	virtual ~LoadingScene() = default;

public:
	virtual void	Init() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;
	virtual void	Final_Tick() override;

private:
	void			Create_LoadingBG();

private:
	shared_ptr<Scene> m_pNextScene = nullptr;
	shared_ptr<Loader> m_pLoader = nullptr;
};

