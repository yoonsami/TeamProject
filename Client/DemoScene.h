#pragma once
#include "Scene.h"
class DemoScene :
    public Scene
{
public:
	DemoScene();
	~DemoScene();

public:
	virtual void Init() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Final_Tick() override;

	virtual HRESULT Load_Scene() override;

private:
	void Load_DemoModel();
	void Load_Camera();
	void Load_Light();
	// 맵 데이터파일의 이름을 넣으면 맵오브젝트들을 생성하는 함수
	HRESULT Load_MapFile(const wstring& _mapFileName);
};

