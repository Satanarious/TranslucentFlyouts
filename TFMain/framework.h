﻿#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <windowsx.h>
#include <winternl.h>
#include <oleacc.h>
#include <taskschd.h>
#include <comutil.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <commctrl.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <vsstyle.h>
#include <vssym32.h>
#include <Imagehlp.h>
#include <TlHelp32.h>
#include <PathCch.h>
#include <delayimp.h>
#include <Psapi.h>
#pragma comment(lib, "Oleacc.lib")
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "windowsapp.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "delayimp.lib")
#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "Imagehlp.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "PathCch.lib")
#pragma comment(lib, "Psapi.lib")