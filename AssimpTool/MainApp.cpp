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

	ExportAssets();
	//ExportParts();
	//ExportWeapon();

	



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

		wstring fileName = entry.path().filename().wstring();
		Utils::DetachExt(fileName);
		if (fileName == L"Player")
			continue;

		if(entry.path().wstring().find(L"Parts") != wstring::npos)
			continue;

		wstring tag = entry.path().wstring();

		shared_ptr<Converter> converter = make_shared<Converter>();
		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		Utils::Replace(tag, L"Assets", L"Models");
		converter->ExportModelData(tag);
		converter->ExportMaterialData(tag);
		converter->ExportAnimationData(tag);
	}
}

void MainApp::ExportParts()
{
	wstring tag = L"..\\Resources\\Assets\\Character\\Player\\Player.fbx";

	shared_ptr<Converter> converter = make_shared<Converter>();
	converter->ReadAssetFile(tag);
	Utils::DetachExt(tag);
	Utils::Replace(tag, L"Assets", L"Models");
	converter->ExportBaseData(tag);

	{
		fs::create_directories(fs::path(tag).parent_path());
		wstring fileName = fs::path(tag).filename();
		wstring finalPath = tag + L".Material";

		shared_ptr<FileUtils> file = make_shared<FileUtils>();
		file->Open(finalPath, FileMode::Write);

		file->Write<_uint>(0);
		
	}
	converter->ExportAnimationData(tag);

	wstring partsPath = L"..\\Resources\\Assets\\Parts\\";

	for (auto& entry : fs::recursive_directory_iterator(partsPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".FBX" && entry.path().extension().wstring() != L".fbx")
			continue;

		wstring tag = entry.path().wstring();
		//wstring typeName = entry.path().parent_path().parent_path().filename().wstring();

		PARTS_INFO eType = PARTS_INFO::END;
		if (tag.find(L"Hair") != wstring::npos)
			eType = PARTS_INFO::Hair;
		else if (tag.find(L"Head") != wstring::npos)
			eType = PARTS_INFO::Head;
		else if (tag.find(L"Uniform") != wstring::npos)
			eType = PARTS_INFO::Uniform;
		else if (tag.find(L"BackParts") != wstring::npos)
			eType = PARTS_INFO::BackParts;
		else
			eType = PARTS_INFO::NonParts;

		converter->ReadAssetFile(tag);
		Utils::DetachExt(tag);
		Utils::Replace(tag, L"Assets\\", L"");
		converter->ExportPartsData(tag, eType);
	}
}
