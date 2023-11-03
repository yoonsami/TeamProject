#pragma once

#include <windows.h>
#include "DirectXMath.h"
#include "SimpleMath.h"

using  _uchar       = unsigned char;
using  _char        = signed char;

using  _ubyte       = unsigned char;
using  _byte        = signed char;

using _ushort       = unsigned short;
using _short        = signed short;

using  _uint        = unsigned int;
using  _int         = signed int;

using _ulong        = unsigned long;
using _long         = signed long;

using _ulonglong    = unsigned long long;
using _longlong     = signed long long;

using _float        = float;
using _double       = double;

using _bool         = bool;
using _tchar        = wchar_t;

using _float2       = DirectX::SimpleMath::Vector2;
using _float3       = DirectX::SimpleMath::Vector3;
using _float4       = DirectX::SimpleMath::Vector4;
using Color         = DirectX::SimpleMath::Vector4;
using _float4x4     = DirectX::SimpleMath::Matrix;

using Quaternion    = DirectX::SimpleMath::Quaternion;
using Ray           = DirectX::SimpleMath::Ray;

using InstanceID    = std::pair<_ulonglong, _ulonglong>;