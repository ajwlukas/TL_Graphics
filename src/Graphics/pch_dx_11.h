﻿#pragma once

#define SAFE_DELETE(x)			  \
{													  \
  if(x != nullptr)								  \
  {													  \
	delete x;									  \
	x = nullptr;								  \
	}												  \
}

#define SAFE_RELEASE(x)		  \
{													  \
  if((x) != nullptr)								  \
  {													  \
	(x)->Release();								  \
	(x) = nullptr;								  \
  }													  \
}

#ifdef WIN32

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK.lib")

#else
//#pragma comment(lib, "Effects11d.lib")
#endif // _DEBUG

#ifdef NDEBUG
#pragma comment(lib, "DirectXTKr.lib")
//#pragma comment(lib, "Effects11.lib")//디버그 나눌것
#endif // NDEBUG
#else
#ifdef _DEBUG
#pragma comment(lib, "DirectXTKx64Debug.lib")
//#pragma comment(lib, "Effects11d.lib")
#endif // _DEBUG

#ifdef NDEBUG
#pragma comment(lib, "DirectXTKx64Release.lib")
//#pragma comment(lib, "Effects11.lib")//디버그 나눌것
#endif // NDEBUG
#endif

//commons
#include <unordered_map>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <corecrt_wstring.h>
#include <assert.h>
#include <memory>
#include <functional>

using namespace std;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

#include "DXTK\d3dx11effect.h"
#include "DXTK\WICTextureLoader\WICTextureLoader.h"
#include "DXTK\DDSTextureLoader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

//#pragma comment(lib, "libfbxsdk.lib")