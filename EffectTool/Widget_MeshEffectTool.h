#pragma once
class Widget_MeshEffectTool
{
public:
	Widget_MeshEffectTool();
	~Widget_MeshEffectTool();

public:
	void	Initialize();
	void	Tick();

private:
	/* Initialize List */
	void					Set_Texture_List();

	/* ImGui Windows */
	void					ImGui_MeshEffectMaker();
};

