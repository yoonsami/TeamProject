#include "pch.h"
#include "Client_Ui_Initializer.h"

#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Utils.h"
#include "BaseUI.h"
#include "FileUtils.h"
#include "Camera.h"
#include "Texture.h"
#include "FontRenderer.h"
#include "CustomFont.h"

void Load_From_UI_Data(const wstring& strDataFilePath)
{
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(strDataFilePath, FileMode::Read);

	_uint iSize = file->Read<_uint>();
	for (_uint i = 0; i < iSize; ++i)
	{
		auto UiObject = make_shared<GameObject>();

		wstring strObjectName = Utils::ToWString(file->Read<string>());
		UiObject->Set_Name(strObjectName);

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		renderer->Set_Mesh(mesh);

		auto material = make_shared<Material>();
		for (_uint i = 0; i < MAX_TEXTURE_MAP_COUONT; ++i)
		{
			_bool bIsUseTexture = file->Read<_bool>();

			if (true == bIsUseTexture)
				material->Set_TextureMap(RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())), static_cast<TextureMapType>(i));
		}

		for (_uint i = 0; i < MAX_SUB_SRV_COUNT; ++i)
		{
			_bool bIsUseSubmap = file->Read<_bool>();

			if (true == bIsUseSubmap)
				material->Set_SubMap(i, RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())));
		}
		renderer->Set_Material(material);

		_float4x4 matWorld = file->Read<_float4x4>();
		UiObject->GetOrAddTransform()->Set_WorldMat(matWorld);

		_uint iPass = file->Read<_uint>();
		renderer->Set_Pass(iPass);

		RenderParams tagParam = file->Read<RenderParams>();
		renderer->Get_RenderParamDesc() = tagParam;
		UiObject->Add_Component(renderer);

		_bool bIsUseBaseUi = file->Read<_bool>();
		if (true == bIsUseBaseUi)
		{
			auto BaseUi = make_shared<BaseUI>();
			BaseUI::BASEUIDESC tagBaseDesc = file->Read<BaseUI::BASEUIDESC>();
			BaseUi->Get_Desc() = tagBaseDesc;
			UiObject->Add_Component(BaseUi);
		}

		_bool bIsUseFont = file->Read<_bool>();
		if (true == bIsUseFont)
		{
			wstring wstrText = Utils::ToWstringUtf8(file->Read<string>());
			auto pFontRenderer = make_shared<FontRenderer>(wstrText);

			wstring strFont = Utils::ToWString(file->Read<string>());
			Color vecColor = file->Read<Color>();
			_float fSize = file->Read<_float>();
			pFontRenderer->Set_Font(RESOURCES.Get<CustomFont>(strFont), vecColor, fSize);

			UiObject->Add_Component(pFontRenderer);
		}

		UiObject->Set_LayerIndex(Layer_UI);
		UiObject->Set_Instancing(false);

		_bool bIsStatic = file->Read<_bool>();
		CUR_SCENE->Add_GameObject(UiObject, bIsStatic);
	}
}
