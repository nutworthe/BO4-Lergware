#include "Present.h"
#include "font.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _M_IX86
uintptr_t discordPresent = 0x14EED4;
uintptr_t discordBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("DiscordHook.dll"));
#elif _M_X64
uintptr_t discordPresent = 0x1B3080;
uintptr_t discordBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("DiscordHook64.dll"));
#endif

void* oPresent;

HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
PresentHookType HookType;
PBYTE present;
__int64 saved_packetDup = 0;
__int64 saved_draw2D = 0;

bool init;

bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

bool should_ignore_msg(UINT msg)
{
	switch (msg)
	{
	case WM_MOUSEACTIVATE:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_NCHITTEST:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHOVER:
	case WM_ACTIVATEAPP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_SETCURSOR:
		return true;
	default:
		return false;
	}
}

void InitImGui()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.Fonts->AddFontFromMemoryTTF(CustomFont, 1, 16);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	ImGui::MergeIconsWithLatestFont(16.f, false);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	const auto should_ignore{ show && !ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) };

	if (should_ignore && should_ignore_msg(uMsg))
	{
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

namespace PresentHook
{
	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (!init)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
			{
				pDevice->GetImmediateContext(&pContext);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				window = sd.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
				pBackBuffer->Release();
				oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
				InitImGui();
				init = true;

				friends::refresh_friends();
			}

			else
				return decltype(&hkPresent)(oPresent)(pSwapChain, SyncInterval, Flags);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			show = !show;
		}

		if (GetAsyncKeyState(VK_F2) & 1)
		{
			Cbuf_AddText(0, "disconnect");
		}

		if (GetAsyncKeyState(VK_F3) & 1)
		{
			Cbuf_AddText(0, "quit");
		}

		friends::on_frame();

		Hook::on_frame();

		*(int*)(dwProcessBase + 0xFC61795) = !show;

		ImGui::GetIO().WantCaptureMouse = true;
		ImGui::GetIO().MouseDrawCursor = show;

		if (show)
		{
			menu::draw();
		}

		overlay::draw();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
		ImGui::RenderNotifications(); // <-- Here we render all notifications
		ImGui::PopStyleVar(1); // Don't forget to Pop()
		ImGui::PopStyleColor(1);

		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		auto p = decltype(&hkPresent)(oPresent)(pSwapChain, SyncInterval, Flags);

		if (GetAsyncKeyState(VK_END) & 1 && HookType == D3D11)
		{
			MH_RemoveHook(MH_ALL_HOOKS);
			MH_DisableHook(MH_ALL_HOOKS);

			*(int*)(dwProcessBase + 0xFC61795) = true;

			*(__int64*)(dwProcessBase + 0xF9FA310) = saved_packetDup;
			*(__int64*)(dwProcessBase + 0x6894FA8) = saved_draw2D;

			if (Live_IsUserInGame(0))
				Cbuf_AddText(0, "disconnect\n");
		}
		return p;
	}
	HookReturn initalize(PresentHookType type)
	{
		HookType = type;

		if (type == DiscordHook)
		{
			if (!discordBase) return ModuleNotFound;

			oPresent = *reinterpret_cast<void**>(discordBase + discordPresent);
			*reinterpret_cast<void**>(discordBase + discordPresent) = hkPresent;
		}
		else if (type == D3D11)
		{
			auto dxgi = (uintptr_t)GetModuleHandleA("dxgi.dll");
			while (!dxgi)
				dxgi = (uintptr_t)GetModuleHandleA("dxgi.dll");

			present = (PBYTE)(dxgi + 0x14D0);

			auto p = MH_CreateHook(present, hkPresent, (void**)&oPresent);
			if (p != MH_OK)
				MessageBox(0, L"Could not create hook", L"Error", MB_OK);
			p = MH_EnableHook(MH_ALL_HOOKS);
			if (p != MH_OK)
				MessageBox(0, L"Could not enable hook", L"Error", MB_OK);
		}

		return Success;
	}
}