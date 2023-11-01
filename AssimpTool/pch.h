// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#ifndef _TOOL_
#define _TOOL_
#endif
#pragma comment(lib,"Engine/Engine.lib")

#include "EnginePch.h"
// Assimp


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#ifdef _DEBUG
#pragma comment(lib,"Assimp/assimp-vc143-mtd.lib")
#else
#pragma comment(lib,"Assimp/assimp-vc143-mt.lib")

#endif // _DEBUG


#include "Assimp_Tool_Define.h"
#include <filesystem>
#include "Utils.h"

#include "FileUtils.h"

namespace fs = std::filesystem;
#endif //PCH_H
