#pragma once
#include "Component.h"

class Material;

class BaseUI :
    public Component
{
public:
    enum class PICK_TYPE
    {
        RECT,
        CIRCLE,
        MAX
    };

public:
    BaseUI();
    virtual ~BaseUI();

    _bool Picked(POINT screenPos);

    void Create(_float2 screenPos, _float2 size, shared_ptr<Material> material);
    void Create(PICK_TYPE eType, POINT ptPos1, POINT ptPos2);

    void AddOnClickedEvent(function<void(void)> func);
    void InvokeOnClicked();

private:
    _bool PtInCircle(POINT screenPos);

private:
    function<void(void)> _onClicked;

    PICK_TYPE   m_eType         = PICK_TYPE::MAX;
    RECT        m_rect          = RECT();
    POINT       m_ptCenter      = {};
    _float      m_fValue        = { 0.f };
};

