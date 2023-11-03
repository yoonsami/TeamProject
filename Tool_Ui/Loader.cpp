#include "pch.h"
#include "Loader.h"

Loader::Loader(shared_ptr<Scene> nextScene)
	:m_pNextScene(nextScene)
{
}

Loader::~Loader()
{
}

_uint APIENTRY ThreadEntry(void* pArg)
{
	Loader* pLoader = (Loader*)pArg;

	(CoInitializeEx(nullptr, 0));
	
	if (FAILED(pLoader->Loading_NextLevel()))
		return 1;

	return 0;
	
}

HRESULT Loader::Init()
{
	InitializeCriticalSection(&m_CriticalSection);

	m_Handle = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (m_Handle == 0)
		return E_FAIL;

	return S_OK;
}

HRESULT Loader::Loading_NextLevel()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = m_pNextScene->Load_Scene();

	
	m_bLoadFinished = true;
	LeaveCriticalSection(&m_CriticalSection);
	return hr;
}
