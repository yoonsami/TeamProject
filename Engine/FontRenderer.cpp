#include "pch.h"
#include "FontRenderer.h"
#include "CustomFont.h"


FontRenderer::FontRenderer(const wstring& text)
	: Component(COMPONENT_TYPE::FontRenderer)
	, m_strText(text)
{


}

void FontRenderer::Tick()
{
	if (m_fTimePerChar == 0.f)
	{
		m_iCurCount = _int(m_strText.size());
		return;

	}

	m_fAcc += fDT;
	if (m_fAcc >= m_fTimePerChar)
	{
		m_iCurCount++;
		if (m_iCurCount >= _int(m_strText.size()))
			m_iCurCount = _int(m_strText.size());
		m_fAcc -= m_fTimePerChar;
	}
}

void FontRenderer::Render()
{
	if (!m_pFont)
		return;

	if (m_iCurCount == 0)
		return;

	wstring text = m_strText.substr(0, m_iCurCount);

	_float3 vPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	vPos.x += GRAPHICS.Get_ViewPort().Get_Width() * 0.5f;
	vPos.y = GRAPHICS.Get_ViewPort().Get_Height() * 0.5f - vPos.y;

	_float2 vLT = {vPos.x - Get_Transform()->Get_Scale().x * 0.5f, vPos.y - Get_Transform()->Get_Scale().y * 0.5f };
	vLT += m_vOffset;
	m_pFont->Get_Batch()->Begin(SpriteSortMode_Deferred);

	m_pFont->Get_Font()->DrawString(m_pFont->Get_Batch().get(), text.c_str(), vLT, m_vColor, 0.f, _float2(0.5f), m_fFontSize);

	m_pFont->Get_Batch()->End();
}

void FontRenderer::Set_Font(shared_ptr<CustomFont> pFont, const Color& vColor, _float fSize)
{
	m_pFont = pFont;
	m_vColor = vColor;
	m_fFontSize = fSize;
}
