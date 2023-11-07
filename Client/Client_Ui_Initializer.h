#pragma once

extern void Load_From_UI_Data(const wstring& strDataFilePath);

#define LOAD_UI_DATA(FILE_PATH) Load_From_UI_Data(FILE_PATH);