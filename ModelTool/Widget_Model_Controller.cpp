#include "pch.h"
#include "Widget_Model_Controller.h"
#include "ModelAnimation.h"
#include "ModelAnimator.h"
#include "Model.h"
#include "ModelRenderer.h"

using namespace ImGui;

bool VectorOfStringGetter(void* data, int n, const char** out_text)
{
	const vector<string>* v = (vector<string>*)data;
	*out_text = (v[0][n]).c_str();
	return true;
}
/* Components */
#include "MeshEffect.h"
#include "Texture.h"
#include "ForcePosition.h"
#include "WeaponScript.h"

Widget_Model_Controller::Widget_Model_Controller()
{
}

Widget_Model_Controller::~Widget_Model_Controller()
{
}

void Widget_Model_Controller::Initialize()
{
	Load_ModelNames();
}

void Widget_Model_Controller::Tick()
{
	m_pControlObject = CUR_SCENE->Get_GameObject(L"TestModel");


	Begin("Model Controller");
	Model_Controller();
	End();

	if (!m_pMaterial.expired())
	{
	Begin("Material Controller");
	Material_Controller();

	End();
	}
}

void Widget_Model_Controller::Load_ModelNames()
{
	wstring assetPath = L"..\\Resources\\Models";
	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".Model" && entry.path().extension().wstring() != L".model")
			continue;

		wstring tag = entry.path().filename().wstring();
		Utils::DetachExt(tag);

		m_ModelNames.push_back(Utils::ToString(tag));
	}
}


void Widget_Model_Controller::Model_Controller()
{
	if (BeginTabBar("##Model Controller"))
	{
		if (BeginTabItem("Model Select"))
		{
			Show_ModelList();
			Apply_Model();

			EndTabItem();
		}

		if(!m_pControlObject.expired())
		{
			if (BeginTabItem("Model Info"))
			{
				Model_Info();

				EndTabItem();
			}
		}
		EndTabBar();
	}
}

void Widget_Model_Controller::Material_Controller()
{
	if (BeginTabBar("##Material Controller"))
	{
		if (BeginTabItem("Material info"))
		{
			Text(Utils::ToString(m_pMaterial.lock()->Get_Name()).c_str());
			
			for (_uint i = 0; i < MAX_TEXTURE_MAP_COUONT; ++i)
			{
				if (m_pMaterial.lock()->Get_TextureMap(static_cast<TextureMapType>(i)))
				{
					Text(Utils::ToString(m_pMaterial.lock()->Get_TextureMap(static_cast<TextureMapType>(i))->Get_Name()).c_str());
				}


			}

			auto& materialDesc = m_pMaterial.lock()->Get_MaterialDesc();
			static bool alpha_preview = true;
			static bool alpha_half_preview = false;
			static bool drag_and_drop = true;
			static bool options_menu = true;
			static bool hdr = false;

			ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
			ImGui::ColorEdit4("ambient", (float*)&materialDesc.ambient, ImGuiColorEditFlags_DisplayHSV | misc_flags);
			ImGui::ColorEdit4("diffuse", (float*)&materialDesc.diffuse, ImGuiColorEditFlags_DisplayHSV | misc_flags);
			ImGui::ColorEdit4("specular", (float*)&materialDesc.specular, ImGuiColorEditFlags_DisplayHSV | misc_flags);
			ImGui::ColorEdit4("emissive", (float*)&materialDesc.emissive, ImGuiColorEditFlags_DisplayHSV | misc_flags);

			if (Button("Save"))
			{
				auto model = m_pControlObject.lock()->Get_Model();
				wstring assetPath = L"..\\Resources\\Models";
				for (auto& entry : fs::recursive_directory_iterator(assetPath))
				{
					if (entry.is_directory())
						continue;

					if (entry.path().extension().wstring() != L".Material")
						continue;

					wstring tag = entry.path().filename().wstring();
					Utils::DetachExt(tag);

					if (tag == model->Get_Name())
					{
						wstring finalPath = entry.path().wstring();

						shared_ptr<FileUtils> file = make_shared<FileUtils>();
						file->Open(finalPath, FileMode::Write);

						file->Write<_uint>(static_cast<_uint>(model->Get_Materials().size()));
						for (auto& material : model->Get_Materials())
						{
							file->Write<string>(Utils::ToString(material->Get_Name()));

							_bool flag = material->Get_TextureMap(TextureMapType::DIFFUSE) != nullptr;
							file->Write<_bool>(flag);
							if (flag)
							{
								file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::DIFFUSE)->Get_Name()));
							}

							flag = material->Get_TextureMap(TextureMapType::SPECULAR) != nullptr;
							file->Write<_bool>(flag);
							if (flag)
							{
								file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::SPECULAR)->Get_Name()));
							}

							flag = material->Get_TextureMap(TextureMapType::NORMAL) != nullptr;
							file->Write<_bool>(flag);
							if (flag)
							{
								file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::NORMAL)->Get_Name()));
							}

							file->Write<Color>(material->Get_MaterialDesc().ambient);
							file->Write<Color>(material->Get_MaterialDesc().diffuse);
							file->Write<Color>(material->Get_MaterialDesc().specular);
							file->Write<Color>(material->Get_MaterialDesc().emissive);
						}



					}


				}


			}

			EndTabItem();
		}



		EndTabBar();
	}
}

void Widget_Model_Controller::Show_ModelList()
{

	SeparatorText("Model List");
	Combo("##Model List", &m_iCurrentModelIndex, VectorOfStringGetter, &m_ModelNames, int(m_ModelNames.size()));
}

void Widget_Model_Controller::Apply_Model()
{
	if (Button("Select Model"))
	{
		CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"TestModel"));

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->Set_Name(L"TestModel");
		obj->GetOrAddTransform();
		CUR_SCENE->Add_GameObject(obj);
		
		
		auto shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
		shared_ptr<Model> model = RESOURCES.Get<Model>(Utils::ToWString(m_ModelNames[m_iCurrentModelIndex]));
		if (model->Get_AnimationCount() == 0)
		{
		
			obj->Add_Component(make_shared<ModelRenderer>(shader));
			obj->Get_ModelRenderer()->Set_Model(model);
		}
		else
		{
			obj->Add_Component(make_shared<ModelAnimator>(shader));
			obj->Get_Animator()->Set_Model(model);
		}
		m_pControlObject = obj;
	}
	if (Button("Specular Zero & Save"))
	{
		auto model = m_pControlObject.lock()->Get_Model();



		wstring assetPath = L"..\\Resources\\Models";
		for (auto& entry : fs::recursive_directory_iterator(assetPath))
		{
			if (entry.is_directory())
				continue;

			if (entry.path().extension().wstring() != L".Material")
				continue;

			wstring tag = entry.path().filename().wstring();
			Utils::DetachExt(tag);

			if (tag == model->Get_Name())
			{
				wstring finalPath = entry.path().wstring();

				shared_ptr<FileUtils> file = make_shared<FileUtils>();
				file->Open(finalPath, FileMode::Write);

				file->Write<_uint>(static_cast<_uint>(model->Get_Materials().size()));
				for (auto& material : model->Get_Materials())
				{
					material->Get_MaterialDesc().specular = _float4(0.f);
					file->Write<string>(Utils::ToString(material->Get_Name()));

					_bool flag = material->Get_TextureMap(TextureMapType::DIFFUSE) != nullptr;
					file->Write<_bool>(flag);
					if (flag)
					{
						file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::DIFFUSE)->Get_Name()));
					}

					flag = material->Get_TextureMap(TextureMapType::SPECULAR) != nullptr;
					file->Write<_bool>(flag);
					if (flag)
					{
						file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::SPECULAR)->Get_Name()));
					}

					flag = material->Get_TextureMap(TextureMapType::NORMAL) != nullptr;
					file->Write<_bool>(flag);
					if (flag)
					{
						file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::NORMAL)->Get_Name()));
					}

					file->Write<Color>(material->Get_MaterialDesc().ambient);
					file->Write<Color>(material->Get_MaterialDesc().diffuse);
					file->Write<Color>(material->Get_MaterialDesc().specular);
					file->Write<Color>(material->Get_MaterialDesc().emissive);
				}
			}
		}
	}

	if (Button("Weak Specular & Save"))
	{
		auto model = m_pControlObject.lock()->Get_Model();



		wstring assetPath = L"..\\Resources\\Models";
		for (auto& entry : fs::recursive_directory_iterator(assetPath))
		{
			if (entry.is_directory())
				continue;

			if (entry.path().extension().wstring() != L".Material")
				continue;

			wstring tag = entry.path().filename().wstring();
			Utils::DetachExt(tag);

			if (tag == model->Get_Name())
			{
				wstring finalPath = entry.path().wstring();

				shared_ptr<FileUtils> file = make_shared<FileUtils>();
				file->Open(finalPath, FileMode::Write);

				file->Write<_uint>(static_cast<_uint>(model->Get_Materials().size()));
				for (auto& material : model->Get_Materials())
				{
					material->Get_MaterialDesc().specular = _float4(0.1f);
					file->Write<string>(Utils::ToString(material->Get_Name()));

					_bool flag = material->Get_TextureMap(TextureMapType::DIFFUSE) != nullptr;
					file->Write<_bool>(flag);
					if (flag)
					{
						file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::DIFFUSE)->Get_Name()));
					}

					flag = material->Get_TextureMap(TextureMapType::SPECULAR) != nullptr;
					file->Write<_bool>(flag);
					if (flag)
					{
						file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::SPECULAR)->Get_Name()));
					}

					flag = material->Get_TextureMap(TextureMapType::NORMAL) != nullptr;
					file->Write<_bool>(flag);
					if (flag)
					{
						file->Write<string>(Utils::ToString(material->Get_TextureMap(TextureMapType::NORMAL)->Get_Name()));
					}

					file->Write<Color>(material->Get_MaterialDesc().ambient);
					file->Write<Color>(material->Get_MaterialDesc().diffuse);
					file->Write<Color>(material->Get_MaterialDesc().specular);
					file->Write<Color>(material->Get_MaterialDesc().emissive);
				}
			}
		}
	}
}

void Widget_Model_Controller::Model_Info()
{
	auto model = m_pControlObject.lock()->Get_Model();

	vector<string> materialNames;
	for (auto& material : model->Get_Materials())
	{
		materialNames.push_back(Utils::ToString(material->Get_Name()));

	}

	static _int curMaterialIndex = 0;
	curMaterialIndex %= materialNames.size();

	Combo("##Material List", &curMaterialIndex, VectorOfStringGetter, &materialNames, int(materialNames.size()));

	m_pMaterial = model->Get_MaterialByName(Utils::ToWString(materialNames[curMaterialIndex]));


}
