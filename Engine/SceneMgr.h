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
		shared_ptr<Scene> preScene = m_pCurrScene;

		if (preScene)
		{
			auto& staticObjects = preScene->Get_StaticObjects();
			m_pCurrScene = scene;
			scene->Init();
			for (auto& staticObject : staticObjects)
			{
				scene->Add_GameObject(staticObject,true);
			}
			

		}
		else
		{
			m_pCurrScene = scene;
			scene->Init();
		}
	}

	shared_ptr<Scene> Get_CurScene() { return m_pCurrScene; }

	void Exchange_Scene()
	{
		if(m_pCurrScene && m_pSubScene)
		m_pCurrScene.swap(m_pSubScene);
	}

	void Add_SubScene(shared_ptr<Scene> scene) { m_pSubScene = scene; m_pSubScene->Init(); }
	void End_SubScene() { m_pCurrScene.swap(m_pSubScene); m_pSubScene = nullptr; }

private:
	shared_ptr<Scene> m_pCurrScene = nullptr;
	shared_ptr<Scene> m_pSubScene = nullptr;
};