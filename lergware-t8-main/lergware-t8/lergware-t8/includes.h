#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <TlHelp32.h>
#include <fstream>
#include <cstdio>
#include <tchar.h>
#include <psapi.h>
#include <random>
#include <algorithm>
#include <minwindef.h>
#include <emmintrin.h>
#include <cstdarg>
#include <cstdint>
#include <chrono>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_notify.h"
#include "present.h"
#include "structs.h"
#include "aimbot.h"
#include "common.h"
#include "spoofing_info/spoofing_info.h"
#include "hooks.h"
#include "exploits.h"
#include "overlay.h"
#include "spoof_call.h"
#include "minhook/include/MinHook.h"
#include "utils/json/nlohmann/json.hpp"
#pragma comment(lib, "d3d11.lib")
#pragma comment( lib, "Wininet.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "wldap32.lib")

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

using namespace std;
using namespace std::literals;
using namespace std::string_literals;
using namespace std::literals::string_literals;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

#include "utils/utils.hpp"
#include "utils/io.hpp"
#include "utils/nt.hpp"

//#include "instant_message.hpp"
#include "friends/friends.hpp"
#include "players.h"
#include "menu.h"
#include "styles.h"

