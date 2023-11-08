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
		if (m_pCurrScene)
		{
			auto& staticObjects = m_pCurrScene->Get_StaticObjects();
			for (auto& staticObject : staticObjects)
			{
				scene->Add_GameObject(staticObject);
			}
			

		}
		m_pCurrScene = scene;
		scene->Init();
	}

	shared_ptr<Scene> Get_CurScene() { return m_pCurrScene; }

private:
	shared_ptr<Scene> m_pCurrScene = nullptr;
};