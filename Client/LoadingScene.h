#pragma once
#include "Scene.h"

class Loader;

class LoadingScene :
    public Scene
{
public:
    LoadingScene(shared_ptr<Scene> nextScene);
    ~LoadingScene();

public:
    virtual void Init() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
    virtual void Final_Tick() override;

private:
    void Load_UI_Texture();
    void Add_UI();

private:
    shared_ptr<Scene> m_pNextScene = nullptr;
    shared_ptr<Loader> m_pLoader = nullptr;
};

