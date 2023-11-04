#include "pch.h"
#include "Widget_ParticleEffectTool.h"

// For. Components
#include "ParticleSystem.h"

Widget_ParticleEffectTool::Widget_ParticleEffectTool()
{
}

Widget_ParticleEffectTool::~Widget_ParticleEffectTool()
{
}

void Widget_ParticleEffectTool::Initialize()
{
	Set_Texture_List();
}

void Widget_ParticleEffectTool::Tick()
{
	ImGui::SetNextWindowPos(ImVec2(g_iWinSizeX - 450, 0));
	ImGui::Begin("Particle Maker");
	ImGui_ParticleMaker();
	ImGui::End();
}

void Widget_ParticleEffectTool::Set_Texture_List()
{
	/* ../Resources/Universal/ 폴더 내의 택스처들의 이름을 리스트로 만들어 저장하기 */ \
	
	m_vecUniversalTextures.push_back("None");

	wstring assetPath = L"..\\Resources\\Textures\\Universal\\";
	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".tga" && entry.path().extension().wstring() != L".png")
			continue;

		string tag = entry.path().string();
		tag = entry.path().filename().string();
		m_vecUniversalTextures.push_back(tag);
	}

	m_iNumUniversalTextures = (_uint)m_vecUniversalTextures.size();
	m_pszUniversalTextures = new const char* [m_iNumUniversalTextures];

	int iIndex = 0;
	for (auto iter : m_vecUniversalTextures)
	{
		m_pszUniversalTextures[iIndex] = m_vecUniversalTextures[iIndex].c_str();
		iIndex++;
	}
}

void Widget_ParticleEffectTool::ImGui_ParticleMaker()
{
	ImGui::SeparatorText("Particle Setting");
	ImGui::InputText("Tag", m_pszParticleTag, MAX_PATH);
	ImGui::Text("");

	/* For. Billbord */
	Option_Billbord();

	/* For. Texture */
	Option_Textures();

	/* For. Color */
	Option_Color();

	/* For. LifeTime */
	Option_LifeTime();

	/* For. Speed */
	Option_Speed();

	/* For. Create, Delete Particle */
	ImGui::Text("");
	ImGui::SeparatorText(" Create/Delete ");
	if (ImGui::Button("Create"))
		Create();
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
		Delete();
}

void Widget_ParticleEffectTool::Option_Textures()
{
	ImGui::SeparatorText("Textures");

	if (ImGui::BeginCombo("Shape", m_pszUniversalTextures[m_iSelected_Texture_Shape], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Shape == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Shape = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Shape];
				m_wstrSelected_Texture_Shape.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Option1##Texture", m_pszUniversalTextures[m_iSelected_Texture_Option1], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Option1 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Option1 = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Option1];
				m_wstrSelected_Texture_Option1.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if(ImGui::BeginCombo("Option2##Texture", m_pszUniversalTextures[m_iSelected_Texture_Option2], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Option2 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Option2 = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Option2];
				m_wstrSelected_Texture_Option2.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Option3##Texture", m_pszUniversalTextures[m_iSelected_Texture_Option3], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Option3 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Option3 = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Option3];
				m_wstrSelected_Texture_Option3.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void Widget_ParticleEffectTool::Option_Color()
{
	ImGui::SeparatorText("Color");

	const char* pszItems[] = { "Use Shape Texture Color", "Constant", "Gradation"};
	if (ImGui::BeginCombo("Options##Color", pszItems[m_iSelected_ColorOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_ColorOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_ColorOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGuiColorEditFlags ColorEdit_flags = 0 | ImGuiColorEditFlags_AlphaBar;	// RGB, Alpha Bar
	switch (m_iSelected_ColorOption)
	{
	case 1: // Constant
		ImGui::ColorEdit4("Selected Color", (float*)&m_vColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	case 2: // Gradation
		ImGui::ColorEdit4("Start Color", (float*)&m_vStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::ColorEdit4("End Color", (float*)&m_vEndColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	}
}

void Widget_ParticleEffectTool::Option_LifeTime()
{
	ImGui::SeparatorText("LifeTime");

	const char* pszItems[] = { "Constant", "Random", "Curve"};
	if (ImGui::BeginCombo("Options##LifeTime", pszItems[m_iSelected_LifeTimeOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_LifeTimeOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_LifeTimeOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	switch (m_iSelected_LifeTimeOption)
	{
	case 0: // Constant
		ImGui::InputFloat("LifeTime(sec)", &m_iLifeTime[0]);
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max LifeTime(sec)", m_iLifeTime);
		break;
	case 2: // Curve
		
		break;
	}
}

void Widget_ParticleEffectTool::Option_Speed()
{
	ImGui::SeparatorText("Speed");

	const char* pszItems[] = { "Constant", "Random", "Curve" };
	if (ImGui::BeginCombo("Options##Speed", pszItems[m_iSelected_SpeedOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_SpeedOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_SpeedOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	switch (m_iSelected_SpeedOption)
	{
	case 0: // Constant
		ImGui::InputFloat("Speed", &m_iSpeed[0]);
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max Speed", m_iSpeed);
		break;
	case 2: // Curve

		break;
	}
}

void Widget_ParticleEffectTool::Option_Billbord()
{
	ImGui::SeparatorText("Billbord");

	//const char* pszItems[] = { "Off", "All", "Horizontal Only", "Vertical Only" };
	const char* pszItems[] = { "Default" };
	if (ImGui::BeginCombo("Options##Speed", pszItems[m_iSelected_BillbordOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_BillbordOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_BillbordOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Widget_ParticleEffectTool::Create()
{
	// For. Particle이 될 게임오브젝트 생성
	shared_ptr<GameObject> ParticleObj = make_shared<GameObject>();

	// For. Setting GameObject 
	ParticleObj->Set_Name(Utils::ToWString(m_pszParticleTag));
	
	// For. GameObject에 transform component 붙이기 + 세팅하기 
	ParticleObj->GetOrAddTransform();
	ParticleObj->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));

	// For. GameObject에 Particle component 붙이기 
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Particle2.fx");
	shared_ptr<ParticleSystem> particleSys =  make_shared<ParticleSystem>(shader);
	ParticleObj->Add_Component(particleSys);	
	ParticleObj->Get_ParticleSystem()->Set_Mesh(RESOURCES.Get<Mesh>(L"Point"));
		
	shared_ptr<Material> material = make_shared<Material>();
	//material->Set_TextureMap(RESOURCES.GetOrAddTexture(L"UniverImg", L"../Resources/Textures/Universal/T_LifeForceOut_Mask_001.png"), TextureMapType::DIFFUSE);
	//material->Get_MaterialDesc().ambient = Color(1.f, 1.f, 1.f, 1.f);
	//material->Get_MaterialDesc().diffuse = Color(1.f, 1.f, 1.f, 1.f);
	//material->Get_MaterialDesc().specular = Color(1.f, 1.f, 1.f, 1.f);
	//material->Get_MaterialDesc().emissive = Color(1.f, 1.f, 1.f, 1.f);
	ParticleObj->Get_ParticleSystem()->Set_Material(material);
	//ParticleObj->Get_ParticleSystem()->Get_ParticleSystemDesc().m_fDuration = 999.f;
	//ParticleObj->Get_ParticleSystem()->Get_ParticleSystemDesc().m_fCreatingTime = 999.f;
	//ParticleObj->Get_ParticleSystem()->Get_ParticleSystemDesc().m_fStartScale = 15.f;
	//ParticleObj->Get_ParticleSystem()->Get_ParticleSystemDesc().m_fEndScale = 15.f;
	//ParticleObj->Get_ParticleSystem()->Get_ParticleSystemDesc().m_fMinLifeTime = 10.f;
	//ParticleObj->Get_ParticleSystem()->Get_ParticleSystemDesc().m_fMaxLifeTime = 10.f;
	//ParticleObj->Get_ParticleSystem()->Get_ParticleSystemDesc().m_iMaxParticle = 30.f;
	//ParticleObj->Get_ParticleSystem()->Get_ComputeParamDesc().SetFloat(0, 1.f);
	//ParticleObj->Get_ParticleSystem()->Get_ComputeParamDesc().SetFloat(1, 1.f);
	//ParticleObj->Get_ParticleSystem()->Get_RenderParamDesc().SetFloat(1, 0.1f);
	//ParticleObj->Get_ParticleSystem()->Get_RenderParamDesc().SetFloat(2, 0.1f);

	ParticleObj->Get_ParticleSystem()->Init();
	// For. 위젯의 Target Object에 바인딩해두기 
	m_pTargetParticle = ParticleObj;

	// For. 씬에 GameObject 추가 
	CUR_SCENE->Add_GameObject(ParticleObj);
}

void Widget_ParticleEffectTool::Delete()
{
	// TODO : m_pTargetParticle에 바인딩 된 GameObject 삭제하기 
}
