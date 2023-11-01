#pragma once
#include "Component.h"
class Material;
class BaseUI :
    public Component
{
public:
    BaseUI();
    virtual ~BaseUI();

    _bool Picked(POINT screenPos);

    void Create(_float2 screenPos, _float2 size, shared_ptr<Material> material);

    void AddOnClickedEvent(function<void(void)> func);
    void InvokeOnClicked();
private:
    function<void(void)> _onClicked;
    RECT _rect = RECT();
};

