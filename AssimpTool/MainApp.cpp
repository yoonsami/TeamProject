#include "pch.h"
#include "MainApp.h"
#include "Converter.h"

MainApp::MainApp()
{
}

MainApp::~MainApp()
{
}

HRESULT MainApp::Init()
{
	{
		GRAPHICDESC			GraphicDesc{};

		GraphicDesc.hWnd = g_hWnd;
		GraphicDesc.iSizeX = g_iWinSizeX;
		GraphicDesc.iSizeY = g_iWinSizeY;
		GraphicDesc.eWinMode = GRAPHICDESC::WINMODE_WIN;

		if (FAILED(GRAPHICS.Initialize(GraphicDesc)))
		{
			MSG_BOX("Engine Init Failed : GRAPHICS");
			return E_FAIL;
		}
	}

	/*{
		wstring tag = L"Normal\\Character\\Gamabunta.fbx";
		shared_ptr<Converter> converter = make_shared<Converter>();
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ExportModelData(tag);
		converter->ExportMaterialData(tag);
		converter->ReadPartsAnimationData(tag);

		tag = L"Normal\\Character\\Gamabunta_etc_Appearance.fbx";
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ReadPartsAnimationData(tag);

		tag = L"Normal\\Character\\Gamabunta_Attack_JumpCrush.fbx";
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ReadPartsAnimationData(tag);

		tag = L"Normal\\Character\\Gamabunta_JumpMove.fbx";
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ReadPartsAnimationData(tag);

		tag = L"Normal\\Character\\Gamabunta_etc_Suggest_Large.fbx";
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ReadPartsAnimationData(tag);

		tag = L"Normal\\Character\\Gamabunta_Ninjutsu_ConsecutiveGunshot.fbx";
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ReadPartsAnimationData(tag);

		tag = L"Normal\\Character\\Gamabunta_Ninjutsu_Gunshot.fbx";
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ReadPartsAnimationData(tag);

		wstring animPath = L"../Resources/Models/Normal\\Character\\Gamabunta.clip";
		converter->WriteAnimationData(animPath);
	}*/

	ExportAssets();				

	return S_OK;
}

void MainApp::Tick()
{
	GAMEINSTANCE. Render_Begin();
	
	GAMEINSTANCE.Render_End();
}

HRESULT MainApp::Render()
{
	
	return S_OK;
}

void MainApp::ExportAssets()
{
	wstring assetPath = L"..\\Resources\\Assets\\"; 	

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".FBX" && entry.path().extension().wstring() != L".fbx")
			continue;

		wstring tag = entry.path().wstring();

		shared_ptr<Converter> converter = make_shared<Converter>();
		converter->ReadAssetFile(tag);
		tag = entry.path().filename().wstring();
		Utils::DetachExt(tag);
		converter->ExportModelData(tag);	
		converter->ExportMaterialData(tag);
		converter->ExportAnimationData(tag);
	}
}

void MainApp::ExportWeapon()
{
	wstring assetPath = L"..\\Resources\\Assets\\Custom\\";

	shared_ptr<Converter> converter = make_shared<Converter>();

	wstring WeaponPath = assetPath + L"Weapon\\";

	for (auto& entry : fs::recursive_directory_iterator(WeaponPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".FBX" && entry.path().extension().wstring() != L".fbx")
			continue;

		wstring tag = entry.path().wstring();

		int i = int(tag.rfind(L"Custom\\"));

		tag = tag.substr(i, tag.size() - i);

		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		converter->ExportModelData(tag);
		converter->ExportMaterialData(tag);
		converter->ExportAnimationData(tag);
	}


}
