#pragma once
#include "ImGuiToolMgr.h"

class Widget_GroupEffectMaker 
{
public:
	Widget_GroupEffectMaker();
	~Widget_GroupEffectMaker();

public:
	void	Initialize();
	void	Tick();

private:
	void	Set_GroupList();
	void	Set_EffectList();

	void	Widget_GroupMaker();
	void	Widget_GetTag();

	void	Option_GroupList();
	void	Option_EffectList();
	void	Option_Effect(string strEffectTag, _int iIndex);

	void	AddNewGroup();
	void	ClearEffectsInGroup(string strGroupTag);

private:
	/* Group List */
	_uint					m_iNumGroups = { 0 };
	vector<string>			m_vecGroups;
	const char**			m_pszGroups = { nullptr };
	_int					m_iGroup = { 0 };
	string					m_strGroup = { "None" };
	
	shared_ptr<GameObject>	m_pCurrentGroup = { nullptr };

	/* Effects List in Group*/
	_uint					m_iNumEffects = { 0 };
	vector<const char*>		m_vecEffects;
	vector<_float>			m_vecEffectTime;
	vector<_int>			m_vecPlayType;

	_int					m_iEffect = { 0 };
	string					m_strEffect = { "None" };

	/* Tag */
	_bool					m_bGetTagWidgetOn = { false };
	char					m_szTag[MAX_PATH] = "-";
	
	
	
};

