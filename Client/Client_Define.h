#pragma once

#include "Client_Enum.h"
#include "Client_Macro.h"
#include "Client_Struct.h"

const unsigned int	g_iWinSizeX = 1920;
const unsigned int	g_iWinSizeY = 1080;

extern HWND		g_hWnd;

#define GET_DATA(VALUE)             GET_SINGLE(DataMgr).Get_Data(VALUE)
#define GET_ELEMENT(VALUE)          GET_SINGLE(DataMgr).Get_Element(GET_SINGLE(DataMgr).Get_Data(VALUE).Element)
#define GET_ELEMENT_LINE(VALUE)     GET_SINGLE(DataMgr).Get_Element_Line(GET_SINGLE(DataMgr).Get_Data(VALUE).Element)