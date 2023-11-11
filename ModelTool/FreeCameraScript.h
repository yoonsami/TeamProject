#pragma once
#include "MonoBehaviour.h"

class FreeCameraScript final : public MonoBehaviour
{
public:
	virtual void	Tick() override;

private:
	_float			m_fRotationSpeed = XM_PI / 4.f;
};

