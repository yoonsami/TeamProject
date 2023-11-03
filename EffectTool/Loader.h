#pragma once

class Scene;
class Loader
{
	friend class LoadingScene;

public:
	Loader(shared_ptr<Scene>nextScene);
	~Loader() = default;

public:
	HRESULT Init();
	HRESULT Loading_NextLevel();

private:
	shared_ptr<Scene>		m_pNextScene = nullptr;
	_bool					m_bLoadFinished = false;
	HANDLE					m_Handle = INVALID_HANDLE_VALUE;
	CRITICAL_SECTION		m_CriticalSection{};
};

