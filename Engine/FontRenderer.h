#pragma once
#include "Component.h"
#include "CustomFont.h"

class FontRenderer : public Component
{
public:
	FontRenderer(const wstring& text);

public:
	void Set_TimePerChar(_float fTimePerChar = 0.f) { m_fTimePerChar = fTimePerChar; m_fAcc = 0.f; m_iCurCount = 0; }
	virtual void Tick() override;
	void Render();

	_bool Is_Finished()					{ return  _int(m_strText.size()) == m_iCurCount; }
	wstring& Get_Text()					{ return m_strText; }
	_float2& Get_Offset()				{ return m_vOffset; }
	Color& Get_Color()					{ return m_vColor; }
	_float& Get_Size()					{ return m_fFontSize; }
	shared_ptr<CustomFont> Get_Font()	{ return  m_pFont; }

	void Set_Font(shared_ptr<CustomFont> pFont, const Color& vColor, _float fSize);
	void Set_Finished()		{ m_iCurCount = _int(m_strText.size()); }

private:
	wstring					m_strText;
	shared_ptr<CustomFont>	m_pFont;

	Color					m_vColor{};
	
	_float					m_fTimePerChar = 0.f;
	_float					m_fAcc = 0.f;
	_int					m_iCurCount = 0;

	_float					m_fFontSize = 1.f;
	_float2					m_vOffset = _float2(1.f);
};

