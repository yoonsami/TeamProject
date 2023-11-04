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

	/* For. Transform */
	Option_Transform();

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
				m_strSelected_Texture_Shape = m_pszUniversalTextures[m_iSelected_Texture_Shape];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	
	if (ImGui::BeginCombo("Dissolve##Texture", m_pszUniversalTextures[m_iSelected_Texture_Dissolve], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Dissolve == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Dissolve = n;
				m_strSelected_Texture_Dissolve = m_pszUniversalTextures[m_iSelected_Texture_Dissolve];
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
				m_strSelected_Texture_Option1 = m_pszUniversalTextures[m_iSelected_Texture_Option1];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Option2##Texture", m_pszUniversalTextures[m_iSelected_Texture_Option2], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Option2 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Option2 = n;
				m_strSelected_Texture_Option2 = m_pszUniversalTextures[m_iSelected_Texture_Option2];
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
		ImGui::ColorEdit4("Selected Color", (float*)&m_vStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		m_vEndColor = m_vStartColor;
		break;
	case 2: // Gradation
		ImGui::ColorEdit4("Start Color", (float*)&m_vStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::ColorEdit4("End Color", (float*)&m_vEndColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	}

	const char* pszItems2[] = { "No Change", "Change" };
	_int iSelected_ChangingColorOption = 0;
	if (ImGui::BeginCombo("Changing Options##Color", pszItems2[iSelected_ChangingColorOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (iSelected_ChangingColorOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				iSelected_ChangingColorOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGuiColorEditFlags ColorEdit_flags = 0 | ImGuiColorEditFlags_AlphaBar;	// RGB, Alpha Bar
	switch (iSelected_ChangingColorOption)
	{
	case 0: // No Change
		m_vDestColor = m_vStartColor;
		break;
	case 1: // Change
		ImGui::ColorEdit4("Dest Color", (float*)&m_vDestColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	}

}

void Widget_ParticleEffectTool::Option_Transform()
{
	ImGui::SeparatorText("Create Position");
	ImGui::InputFloat3("Center Position", m_fCenterPosition);
	ImGui::InputFloat3("Range", m_fCreateRange);

	ImGui::SeparatorText("Scale");
	const char* pszItems[] = { "Constant", "Random", "Curve" };
	if (ImGui::BeginCombo("Options##Scale", pszItems[m_iSelected_ScaleOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_ScaleOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_ScaleOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	switch (m_iSelected_ScaleOption)
	{
	case 0: // Constant
		ImGui::InputFloat("Scale", &m_iScale[0]);
		m_iScale[1] = m_iScale[0];
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max Scale", m_iScale);
		break;
	case 2: // Curve
		ImGui::InputFloat2("Base, Exponent Scale", m_iScale);
		break;
	}
	
	ImGui::SeparatorText("Rotation");
	ImGui::InputFloat3("Range", m_fRotationSpeed);
	
	const char* pszItems2[] = { "Constant", "Random" };
	if (ImGui::BeginCombo("Options##RotateAngle", pszItems2[m_iSelected_RotationAngleOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems2); n++)
		{
			const bool is_selected = (m_iSelected_RotationAngleOption == n);
			if (ImGui::Selectable(pszItems2[n], is_selected))
				m_iSelected_RotationAngleOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	switch (m_iSelected_RotationAngleOption)
	{
	case 0: // Constant
		ImGui::InputFloat3("x,y,z Angle", m_fRotationAngle);
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max Angle", m_fRotationAngle);
		break;
	}
}

void Widget_ParticleEffectTool::Option_LifeTime()
{
	ImGui::SeparatorText("LifeTime");

	const char* pszItems[] = { "Constant", "Random" };
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
		m_iLifeTime[1] = m_iLifeTime[0];
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max LifeTime(sec)", m_iLifeTime);
		break;
	}
}

void Widget_ParticleEffectTool::Option_Movement()
{
	const char* pszItems[] = { "Default" };
	if (ImGui::BeginCombo("Options##Movement", pszItems[m_iSelected_MovementOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_MovementOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_MovementOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
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
		m_iSpeed[1] = m_iSpeed[0];
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max Speed", m_iSpeed);
		break;
	case 2: // Curve
		ImGui::InputFloat2("Base, Exponent Speed", m_iSpeed);
		break;
	}
}

void Widget_ParticleEffectTool::Option_Billbord()
{
	ImGui::SeparatorText("Billbord");

	const char* pszItems[] = { "Off", "All", "Horizontal Only", "Vertical Only" };
	if (ImGui::BeginCombo("Options##Billbord", pszItems[m_iSelected_BillbordOption], 0))
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
	
	ParticleObj->Get_ParticleSystem()->Set_Material(material);
	
	// TODO 

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
