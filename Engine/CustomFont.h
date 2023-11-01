#pragma once
#include "ResourceBase.h"

class CustomFont : public ResourceBase
{
public:
	CustomFont();

	virtual void Load(const wstring& strPath) override;


	shared_ptr<SpriteBatch> Get_Batch() { return m_pBatch; }
	shared_ptr<SpriteFont> Get_Font() { return m_pFont; }

private:
	shared_ptr<SpriteBatch> m_pBatch;
	shared_ptr<SpriteFont> m_pFont;


};

