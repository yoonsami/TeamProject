#include "pch.h"
#include "Smoke_WitcherSense.h"
#include "GroupEffect.h"
#include "UiQuestController.h"

Smoke_WitcherSense::Smoke_WitcherSense(shared_ptr<GameObject> pPlayer)
	: m_pPlayer(pPlayer)
{
}

void Smoke_WitcherSense::Tick()
{
	if (m_pPlayer.expired() || m_pOwner.expired())
		return;

	{
		auto obj = CUR_SCENE->Get_UI(L"UI_Dialog_Controller");
		if (obj && !(obj->Get_Script<UiQuestController>()->Get_CurState(QUESTINDEX::HIDE_AND_SEEK) == CUR_QUEST::PROGRESS))
		{
			if (Get_Owner()->Get_GroupEffect())
			{

				Get_Owner()->Get_GroupEffect()->Set_TickRenderState(false, false);
			}

		}
		else
		{
			if (Get_Owner()->Get_GroupEffect())
			{

				Get_Owner()->Get_GroupEffect()->Set_TickRenderState(g_bWitcherSenseOn, g_bWitcherSenseOn);
			}

		}


	}
}
