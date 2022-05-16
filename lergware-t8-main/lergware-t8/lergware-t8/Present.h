#pragma once
#include "includes.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern uintptr_t discordPresent;
extern uintptr_t discordBase;

extern int my_image_width;
extern int my_image_height;
extern ID3D11ShaderResourceView* my_texture;

extern __int64 saved_packetDup;
extern __int64 saved_draw2D;

enum PresentHookType
{
	DiscordHook,
	D3D11
};

enum HookReturn
{
	Success,
	ModuleNotFound,
	D3D11CreateDeviceAndSwapChainFailed
};

namespace PresentHook
{
	HookReturn initalize(PresentHookType type);
}