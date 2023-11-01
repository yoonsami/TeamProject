#include "pch.h"
#include "CustomFont.h"

CustomFont::CustomFont()
	: ResourceBase(ResourceType::Font)
{
}

void CustomFont::Load(const wstring& strPath)
{
	m_pBatch = make_shared<SpriteBatch>(CONTEXT.Get());

	m_pFont = make_shared<SpriteFont>(DEVICE.Get(), strPath.c_str());
}
