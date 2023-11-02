#include "pch.h"
#include "Widget_ParticleEffectTool.h"

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
}

void Widget_ParticleEffectTool::ImGui_ParticleMaker()
{
	ImGui::Text("Test");
}
