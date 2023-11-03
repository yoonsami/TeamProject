#include "pch.h"
#include "Widget_MeshEffectTool.h"

Widget_MeshEffectTool::Widget_MeshEffectTool()
{
}

Widget_MeshEffectTool::~Widget_MeshEffectTool()
{
}

void Widget_MeshEffectTool::Initialize()
{
	Set_Texture_List();
}

void Widget_MeshEffectTool::Tick()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Mesh Effect Maker");
	ImGui_MeshEffectMaker();
	ImGui::End();
}

void Widget_MeshEffectTool::Set_Texture_List()
{
}

void Widget_MeshEffectTool::ImGui_MeshEffectMaker()
{
	ImGui::Text("Mesh Effect Maker");
}
