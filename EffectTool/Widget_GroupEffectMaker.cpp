#include "pch.h"
#include "Widget_GroupEffectMaker.h"

#include "GameObject.h"

Widget_GroupEffectMaker::Widget_GroupEffectMaker()
{
}

Widget_GroupEffectMaker::~Widget_GroupEffectMaker()
{
}

void Widget_GroupEffectMaker::Set_GroupList()
{
	m_vecGroups.push_back("None");

	wstring assetPath = L"..\\Resources\\EffectData\\GroupEffectData\\";
	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".dat")
			continue;

		string tag = entry.path().string();
		tag = entry.path().filename().string();
		m_vecGroups.push_back(tag);
	}

	m_iNumGroups = (_uint)m_vecGroups.size();
	m_pszGroups = new const char* [m_iNumGroups];

	int iIndex = 0;
	for (auto iter : m_vecGroups)
	{
		m_pszGroups[iIndex] = m_vecGroups[iIndex].c_str();
		iIndex++;
	}
}

void Widget_GroupEffectMaker::Set_EffectList()
{
}

void Widget_GroupEffectMaker::Widget_GroupMaker()
{
	if (m_bGetTagWidgetOn)
		Widget_GetTag();
}

void Widget_GroupEffectMaker::Widget_GetTag()
{
	ImGui::SeparatorText("Group Name");

	ImGui::InputText("##GetTag", m_szTag, MAX_PATH);

	if (ImGui::Button("Create##GetTag"))
		m_bGetTagWidgetOn = false;
}

void Widget_GroupEffectMaker::Option_GroupList()
{
	/* If select Group 
		1. Erase all Effect in current group 
		2. Create all Effect in selected group 
		3. Set all Effect's Play type to "Play Loop" */

	ImGui::Text("Group List");
	if(ImGui::BeginCombo("##GroupEffectList", m_pszGroups[m_iGroup], 0))
	{
		for (_uint n = 0; n < m_iNumGroups; n++)
		{
			const bool is_selected = (m_iGroup == n);
			if (ImGui::Selectable(m_pszGroups[n], is_selected))
			{
				// 1. Erase all Effect in current group 
				ClearEffectsInGroup(m_strGroup);

				// 2. Add GroupEffect to Current Scene  
				m_iGroup = n;
				m_strGroup = m_pszGroups[m_iGroup];
				m_pCurrentGroup = RESOURCES.Get<MeshEffectData>(m_pszGroups[m_iGroup], CUR_SCENE);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void Widget_GroupEffectMaker::Option_EffectList()
{

}

void Widget_GroupEffectMaker::Option_Effect(string strEffectTag, _int iIndex)
{
	string strKey_Tree = strEffectTag + to_string(iIndex) + "##Effect";
	string strKey_RadioButton1 = "Play Once##" + strEffectTag + to_string(iIndex);
	string strKey_RadioButton2 = "Play Loop##" + strEffectTag + to_string(iIndex);
	string strKey_RadioButton3 = "Show First Frame##" + strEffectTag + to_string(iIndex);
	string strKey_InputFloat = "Time##" + strEffectTag + to_string(iIndex);

	if (ImGui::TreeNode(strKey_Tree.c_str()))
	{
		ImGui::InputFloat("Time##Effet", &m_vecEffectTime[iIndex]);

		if (ImGui::RadioButton(strKey_RadioButton1.c_str(), &m_vecPlayType[iIndex], 0))
		{
			shared_ptr<Shader> shader = RESOURCES.Get<GameObject>(string(m_vecEffects[iIndex]));
		} ImGui::SameLine();
		if (ImGui::RadioButton(strKey_RadioButton2.c_str(), &m_vecPlayType[iIndex], 1))
		{

		} ImGui::SameLine();
		if (ImGui::RadioButton(strKey_RadioButton3.c_str(), &m_vecPlayType[iIndex], 2))
		{

		}
		ImGui::TreePop();
	}
}

void Widget_GroupEffectMaker::AddNewGroup()
{
	m_bGetTagWidgetOn = true;
}

void Widget_GroupEffectMaker::ClearEffectsInGroup(string strGroupTag)
{
}

void Widget_GroupEffectMaker::Initialize()
{
}

void Widget_GroupEffectMaker::Tick()
{
}
