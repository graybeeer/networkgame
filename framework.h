// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <mmsystem.h>

#include <atlimage.h>
#include <random>
#include <cmath>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <unordered_map>

#include <crtdbg.h>
#include <assert.h>

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define SDF _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else 
#define SDF NULL
#define new new
#endif

#ifdef _DEBUG 
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif

#pragma warning(disable: 6387)
#pragma warning(disable: 26541)

#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

const int MD_NULL = 0;
const int MD_UP = 1;
const int MD_DOWN = 2;
const int MD_LEFT = 3;
const int MD_RIGHT = 4;

using namespace std;

