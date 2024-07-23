#include "framework.h"


// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectXTK.lib")
#if defined(_DEBUG)
#pragma comment( lib, "dxguid.lib")
#endif
//For Windows 11 D3D 12 debugger problems, uncomment the following.
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 611; }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }
