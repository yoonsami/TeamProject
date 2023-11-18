#pragma once

template<typename>
class Geometry;
class MainApp
{
public:
	MainApp();
	virtual ~MainApp();

public:
	HRESULT Init();
	void	Tick();
	HRESULT Render();

private:
	void	ExportAssets();			// MEMO: 기본적인 모델 로드시 활성화
	void	ExportParts();

	void	ExportWrongAnimRight();
};

