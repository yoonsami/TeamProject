#pragma once
#include "ImGuiToolMgr.h"

class Widget_Model_Controller
{
public:
	Widget_Model_Controller();
	~Widget_Model_Controller();

public:
	void	Initialize();
	void	Tick();


private:
	/* Initialize List */
	void Load_ModelNames();


	/* ImGui Windows */
	void Model_Controller();
	void Material_Controller();
	void Parts_Controller();


	/* Funtions */
	void Show_ModelList();
	void Apply_Model();

	void Model_Info();

	void Show_AnimationList();
	void Apply_Anim();

	void Control_Anim();
private:
	//Model Name
	_int m_iCurrentModelIndex = 0;
	vector<string> m_ModelNames;

	//Animation Name
	_int m_iCurrentAnimIndex = 0;
	vector<string> m_AnimNames;

	weak_ptr<GameObject> m_pControlObject;

	weak_ptr<Material> m_pMaterial;
};

