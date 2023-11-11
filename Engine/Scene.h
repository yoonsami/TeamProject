#pragma once
#include "BindShaderDesc.h"

class GameObject;

class Scene
{
public:
	enum SCENE_STATE
	{
		SCENE_READY,
		SCENE_PLAY,
		SCENE_DISAPPEAR,
		SCENE_NONE,
	};

public:
	Scene();
	~Scene();

public:
	virtual void Init();
	virtual void Tick();
	virtual void Late_Tick();
	virtual void Final_Tick();
	virtual void Render();
	virtual HRESULT Load_Scene();
	virtual void Add_GameObject(shared_ptr<GameObject> object, _bool staticFlag = false);
	virtual void Remove_GameObject(shared_ptr<GameObject> object);

public:
	_float Get_LoaingPercent() { return m_fLoadPercent; }
	void Set_Name(const wstring& name) { m_strSceneName = name; }
	wstring Get_Name() { return m_strSceneName; }
	vector<shared_ptr<GameObject>>& Get_Objects() { return m_GameObjects; }
	vector<shared_ptr<GameObject>>& Get_StaticObjects() { return m_StaticObject; }
	shared_ptr<GameObject> Get_Camera(const wstring& cameraName);
	shared_ptr<GameObject> Get_MainCamera();
	shared_ptr<GameObject> Get_UICamera();

	shared_ptr<GameObject> Get_Light() { return m_Lights.empty() ? nullptr : *m_Lights.begin(); }
	shared_ptr<GameObject> Get_GameObject(const wstring& name);
	_bool Is_Static(shared_ptr<GameObject> obj);

	shared_ptr<GameObject> Get_UI(const wstring& strName);
	LightParams& Get_LightParams() { return m_LightParams; }

	void Swap_Object(const wstring& leftObjName, const wstring& rightObjName);
	SCENE_STATE Get_SceneState() { return m_eSceneState; }
	void Set_SceneState(SCENE_STATE state) { m_eSceneState = state; }
	void Render_ToneMapping();

protected:
	void Load_SceneFile(const wstring& sceneName);
	void Load_UIFile(const wstring& path);
	void Load_MapFile(const wstring& _mapFileName);
	void PickUI();

protected:
	void Gather_LightData();
	void Sort_GameObjects();
	void Render_Shadow();
	void Render_MotionBlur();
	void Render_Deferred();
	void Render_DefferedBlur();
	void Render_SSAO();

	void Render_SSAOBlur(_uint blurCount);

	void Render_Lights();
	void Render_LightFinal();

	void Render_OutLine();

	void Render_MotionBlurFinal();

	void Render_Forward();

	void Render_BloomMap();
	void Render_BloomMapScaling();


	void Render_Distortion();
	void Render_Distortion_Final();


	void Render_LensFlare();

	void Render_Fog();
	void Render_Aberration();

	void Render_Debug();


	void Render_UI();

	void Render_FXAA();

	void Render_BackBuffer();



private:
	void SSAO_MakeOffsetVector();
	void SSAO_MakeFrustumFarCorners();

protected:
	_float m_fLoadPercent = 0.f;
	wstring m_strSceneName = L"";
	vector<shared_ptr<GameObject>> m_GameObjects;
	wstring m_strSceneDataPath = L"../Resources/Scene/";
	//Cache
	vector<shared_ptr<GameObject>> m_Cameras;
	//Cache
	vector<shared_ptr<GameObject>> m_Lights;
	vector<shared_ptr<GameObject>> m_UI;

	LightParams m_LightParams;
	SCENE_STATE m_eSceneState = SCENE_READY;

	_bool m_bSceneFinished = false;

	_uint m_iLevelIndex = 0;
	_float4 m_vFrustumFarCorner[4];
	_float4 m_vOffsets[14];

protected:
	vector<shared_ptr<GameObject>> m_StaticObject;

	wstring m_wstrFinalRenderTarget = L"";



	_bool m_bRenderDebug = false;
};

