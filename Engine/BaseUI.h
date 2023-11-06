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

    typedef struct tagBaseUiDesc {
        _float      fValue;
        POINT       ptCenter;
        RECT        rect;
        PICK_TYPE   eType;
    }BASEUIDESC;

public:
    BaseUI();
    virtual ~BaseUI();

    _bool Picked(POINT screenPos);

    void Create(_float2 screenPos, _float2 size, shared_ptr<Material> material);
    void Create(PICK_TYPE eType, POINT ptPos1, POINT ptPos2);

    void AddOnClickedEvent(function<void(void)> func);
    void InvokeOnClicked();

    BASEUIDESC& Get_Desc() { return m_tagDesc; }

private:
    _bool PtInCircle(POINT screenPos);

private:
    function<void(void)> _onClicked;

    BASEUIDESC  m_tagDesc = {};
};

