#include "pch.h"
#include "MapObjectScript.h"
MapObjectScript::MapObjectScript(MAPOBJDESC _MapObjDesc)
{
	m_MapObjDesc = _MapObjDesc;
}

HRESULT MapObjectScript::Init()
{
	return S_OK;
}

void MapObjectScript::Tick()
{
}
