#pragma once
#include "MonoBehaviour.h"

class GroupEffect;
class GroupEffectScript final : public MonoBehaviour
{
public:

	virtual HRESULT Init() override;
	virtual void	Tick() override;

	/* Setter */
	void			Set_GroupEffect();

	/* Getter */
	
private:
	shared_ptr<GroupEffect>	m_pGroupEffectCom;
};

