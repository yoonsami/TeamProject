#include "pch.h"
#include "SceneMgr.h"

void SceneMgr::Tick()
{
	if (m_pCurrScene == nullptr)
		return;

	m_pCurrScene->Tick();
	


}

void SceneMgr::Late_Tick()
{
	if (m_pCurrScene == nullptr)
		return;

	m_pCurrScene->Late_Tick();
}

void SceneMgr::Final_Tick()
{
	if (m_pCurrScene == nullptr)
		return;

	m_pCurrScene->Final_Tick();
}

void SceneMgr::Render()
{
	if (m_pCurrScene == nullptr)
		return;

	m_pCurrScene->Render();
}
