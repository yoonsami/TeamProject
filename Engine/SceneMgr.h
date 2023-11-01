#pragma once 
#include "Scene.h"


class SceneMgr
{
	DECLARE_SINGLETON(SceneMgr)
public:
	void Tick();
	void Late_Tick();
	void Final_Tick();
	void Render();
	template<typename T>
	void Change_Scene(shared_ptr<T> scene)
	{
		m_pCurrScene = scene;
		scene->Init();
	}

	shared_ptr<Scene> Get_CurScene() { return m_pCurrScene; }

private:
	shared_ptr<Scene> m_pCurrScene = nullptr;

};

