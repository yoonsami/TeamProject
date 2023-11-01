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
	void Tick();
	HRESULT Render();

private:
	void ExportAssets();


	void ExportWeapon();
};

