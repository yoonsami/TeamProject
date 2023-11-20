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
	static _bool checkBox = true;
	Checkbox("SkyBox On", &checkBox);
	if (CUR_SCENE->Get_GameObject(L"SkyBase"))
		CUR_SCENE->Get_GameObject(L"SkyBase")->Get_ModelRenderer()->Set_RenderState(checkBox);
	Model_Controller();
	End();

	if (!m_pMaterial.expired())
	{
		Begin("Material Controller");
		Material_Controller();

		End();
	}

	if (!m_pControlObject.expired() && m_pControlObject.lock()->Get_Model()->Get_ModelTag() == L"Player")
	{
		Begin("Parts Control");
		Parts_Controller();

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
			if (BeginTabItem("Animation Select"))
			{
				m_pControlObject.lock()->Get_Animator()->Set_AnimState(false);
				Show_AnimationList();
				Apply_Anim();
				EndTabItem();
			}
			if (BeginTabItem("Animation Control"))
			{
				m_pControlObject.lock()->Get_Animator()->Set_AnimState(true);
				Control_Anim();

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
			
			/*for (_uint i = 0; i < MAX_TEXTURE_MAP_COUONT; ++i)
			{
				if (m_pMaterial.lock()->Get_TextureMap(static_cast<TextureMapType>(i)))
				{
					Text(Utils::ToString(m_pMaterial.lock()->Get_TextureMap(static_cast<TextureMapType>(i))->Get_Name()).c_str());
				}


			}*/

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

void Widget_Model_Controller::Parts_Controller()
{
	vector<string> partsNames[PARTS_MAX_COUNT];

	for(_uint i =0; i< PARTS_MAX_COUNT; ++i)
	{
		const auto& parts =	RESOURCES.Get_Parts(static_cast<PARTS_INFO>(i));
		for (auto& pair : parts)
		{
			partsNames[i].push_back(Utils::ToString(pair.first));
		}
	}
	static _int partsIndex[PARTS_MAX_COUNT] = {};

	for(_int i =0; i< PARTS_MAX_COUNT; ++i)
	{
		string tag = "##PartsList" + to_string(i);
		Combo(tag.c_str(), &partsIndex[i], VectorOfStringGetter, &partsNames[i], int(partsNames[i].size()));
		SameLine();
		string buttonName = "Set Parts ##" + to_string(i);
		if (Button(buttonName.c_str()))
		{
			auto model = m_pControlObject.lock()->Get_Model();
			model->AddParts(Utils::ToWString(partsNames[i][partsIndex[i]]), static_cast<PARTS_INFO>(i));
		}
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
		if (m_pControlObject.expired())
		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->Set_Name(L"TestModel");
			obj->GetOrAddTransform();
			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(RESOURCES.Get<Shader>(L"Shader_Model.fx"));
			obj->Add_Component(animator);
			CUR_SCENE->Add_GameObject(obj);
			m_pControlObject = obj;
		}

		shared_ptr<Model> model = RESOURCES.Get<Model>(Utils::ToWString(m_ModelNames[m_iCurrentModelIndex]));
		m_pControlObject.lock()->Get_Animator()->Set_Model(model);
		m_pControlObject.lock()->Get_Animator()->Set_CurrentAnim(0);
		m_pControlObject.lock()->Add_Component(make_shared<ForcePosition>());

		CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"TestWeapon"));

		wstring weaponName;
		if (model->Get_ModelTag() == L"Spear_Ace")
			weaponName = L"Weapon_Spear_Ace";
		else if (model->Get_ModelTag() == (L"Yeopo"))
			weaponName = L"Weapon_Yeopo";
		else if (model->Get_ModelTag() == (L"Dellons"))
			weaponName = L"Weapon_Dellons";
		else if (model->Get_ModelTag() == (L"Rachel"))
			weaponName = L"Weapon_Rachel";
		else if (model->Get_ModelTag() == (L"Shane"))
			weaponName = L"Weapon_Shane";

		if (model->Get_ModelTag() != (L"Kyle") && model->Get_ModelTag() != (L"Yeonhee"))
		{
			shared_ptr<GameObject> ObjWeapon = make_shared<GameObject>();
			ObjWeapon->Set_Name(L"TestWeapon");
			ObjWeapon->GetOrAddTransform();
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
			shared_ptr<Model> m = RESOURCES.Get<Model>(weaponName);
			renderer->Set_Model(m);
			ObjWeapon->Add_Component(renderer);
			WeaponScript::WEAPONDESC desc;
			desc.strBoneName = L"Bip001-Prop1";
			desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
			desc.pWeaponOwner = m_pControlObject.lock();
			ObjWeapon->Add_Component(make_shared<WeaponScript>(desc));
			CUR_SCENE->Add_GameObject(ObjWeapon);
		}
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

void Widget_Model_Controller::Show_AnimationList()
{
	auto& animations = m_pControlObject.lock()->Get_Animator()->Get_Model()->Get_Animations();

	m_AnimNames.clear();
	m_AnimNames.resize(animations.size());

	for (_uint i = 0; i < static_cast<_uint>(animations.size()); ++i)
		m_AnimNames[i] = Utils::ToString(animations[i]->name);

	SeparatorText("Anim List");
	ListBox("##Anim List", &m_iCurrentAnimIndex, VectorOfStringGetter, &m_AnimNames, int(m_AnimNames.size()));

}

void Widget_Model_Controller::Apply_Anim()
{
	static _float fAnimSpeed = 1.f;
	static _bool bAnimLoop = false;
	static _bool bSetPosition = false;

	DragFloat("Anim Speed", &fAnimSpeed, 0.05f, 0.1f, 5.f);
	Checkbox("Anim Loop", &bAnimLoop);
	if (Button("Select Animation"))
		m_pControlObject.lock()->Get_Animator()->Set_CurrentAnim(Utils::ToWString(m_AnimNames[m_iCurrentAnimIndex]), bAnimLoop, fAnimSpeed);

	Checkbox("Position Force To Zero", &bSetPosition);
	m_pControlObject.lock()->Get_Script<ForcePosition>()->bForcePosition = bSetPosition;
}

void Widget_Model_Controller::Control_Anim()
{
	auto animator = m_pControlObject.lock()->Get_Animator();

	auto& tweenDesc = animator->Get_TweenDesc();

	const auto& animation = animator->Get_Model()->Get_AnimationByIndex(tweenDesc.curr.animIndex);

	static _float fKeyFrame = 0.f;

	DragFloat("## anim", &fKeyFrame, 0.1f, 0.f, _float(animation->frameCount -1));

	_uint curFrame = _uint(fKeyFrame);
	_uint nextFrame = curFrame + 1;
	if (nextFrame >= animation->frameCount-1) nextFrame = animation->frameCount-1;

	tweenDesc.curr.currentFrame = curFrame;
	tweenDesc.curr.nextFrame = nextFrame;
	tweenDesc.curr.ratio = fKeyFrame - curFrame;
}
