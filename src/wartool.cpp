#include "wartool.h"
#include "CImguiMgr.h"
#include "CLockCursor.h"
#include "CDiscordRPC.h"

// OpenGL
_wglSwapLayerBuffers ORIG_wglSwapLayerBuffers = NULL;
_wglGetProcAddress ORIG_wglGetProcAddress = NULL;
_glClear ORIG_glClear = NULL;
_WndProc ORIG_WndProc = NULL;

// Engine
_SetGameAreaFOV ORIG_SetGameAreaFOV = NULL;
float g_GetWindowXoffset;
float g_GetWindowYoffset;

void* g_lpOpenGL32;
void* g_lpGameDLL;
CImguiMgr gImGui;
CLockCursor gLockCursor;
CDiscordRPC gDiscordRPC;
HWND gHwnd;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
inline long __stdcall HOOKED_WndProc(const HWND a1, unsigned int a2, unsigned a3, long a4)
{
	ImGui_ImplWin32_WndProcHandler(a1, a2, a3, a4);
	return CallWindowProcA(ORIG_WndProc, a1, a2, a3, a4);
}

_wglSwapLayerBuffers GetSwapLayerBuffersAddr()
{
	return reinterpret_cast<_wglSwapLayerBuffers>(GetProcAddress(LoadLibrary(TEXT("OpenGL32.dll")), "wglSwapLayerBuffers"));
}

_wglGetProcAddress GetWGLProcAddress()
{
	return reinterpret_cast<_wglGetProcAddress>(GetProcAddress(LoadLibrary(TEXT("OpenGL32.dll")), "wglGetProcAddress"));
}

_glClear GetGLClearAddr()
{
	return reinterpret_cast<_glClear>(GetProcAddress(LoadLibrary(TEXT("OpenGL32.dll")), "glClear"));
}

int __stdcall HOOKED_wglSwapLayerBuffers(HDC a1, UINT a2)
{
    static bool initialized = false;

    if (!initialized)
    {
        gHwnd = WindowFromDC(a1);

        ORIG_WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(gHwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HOOKED_WndProc)));;

        gImGui.Init(gHwnd);
		gLockCursor.SetWindow(gHwnd);

		gDiscordRPC.Init();
		gDiscordRPC.SetGameState(INMENU);

        initialized = true;
    }

    gImGui.End();

	// TODO: lock only if in the going game - keyboardcrash
	if (gImGui.mainMenu.lockCursor && !gLockCursor.cursorLocked)
		gLockCursor.Lock();

	if (!gImGui.mainMenu.lockCursor && gLockCursor.cursorLocked)
		gLockCursor.Unlock();

    return ORIG_wglSwapLayerBuffers(a1, a2);
}

void __stdcall HOOKED_glClear(GLbitfield a1)
{
	gImGui.Draw();

    ORIG_glClear(a1);
}

PROC __stdcall HOOKED_wglGetProcAddress(LPCSTR a1) // just for logging
{
    printf("[OpenGL32] HOOKED_wglGetProcAddress: %s\n", a1);
    return ORIG_wglGetProcAddress(a1);
}

int __fastcall HOOKED_SetGameAreaFOV(Matrix1* a1, int a2, float a3, float a4, float a5, float a6)
{
	g_GetWindowXoffset = *(float*)MakePtr(g_lpGameDLL, GETWINDOWXOFFSET_OFFSET);
	g_GetWindowYoffset = *(float*)MakePtr(g_lpGameDLL, GETWINDOWYOFFSET_OFFSET);

	if (g_GetWindowXoffset == 0 || g_GetWindowYoffset == 0)
		return ORIG_SetGameAreaFOV(a1, a2, a3, a4, a5, a6);

	const float CustomFovFix = 1.0f;

	float ScreenX = g_GetWindowXoffset;
	float ScreenY = g_GetWindowYoffset;

	float v1 = 1.0f / sqrt(a4 * a4 + 1.0f);
	float v2 = tan(v1 * a3 * 0.5f);

	float v3 = v2 * a5;
	float v4 = v3 * a4;


	a1->flt1 = ((a5 * (4.0f / 3.0f)) / (ScreenX / ScreenY) * CustomFovFix) / v4; // Fix 4:3 to WindowX/WindowY
	a1->flt2 = 0.0f;
	a1->flt3 = 0.0f;
	a1->flt4 = 0.0f;
	a1->flt5 = 0.0f;


	a1->flt6 = a5 / v3;
	a1->flt7 = 0.0f;
	a1->flt8 = 0.0f;
	a1->flt9 = 0.0f;
	a1->flt10 = 0.0f;


	a1->flt11 = (a5 + a6) / (a6 - a5);
	a1->flt12 = 1.0f;
	a1->flt13 = 0.0f;
	a1->flt14 = 0.0f;


	a1->flt15 = a5 * (a6 * -2.0f) / (a6 - a5);
	a1->flt16 = 0.0f;

	//return ORIG_SetGameAreaFOV(a1, a2, a3, a4, a5, a6); // no need in this?
	return 0;
}

void HookOpenGL()
{
    g_lpOpenGL32 = GetModuleHandleA("opengl32.dll");
    if (!g_lpOpenGL32)
        return;

    ORIG_wglSwapLayerBuffers = GetSwapLayerBuffersAddr();
    ORIG_glClear = GetGLClearAddr();

    int status;

	Find(OpenGL32, glClear);
	CreateHook(OpenGL32, glClear);
    Find(OpenGL32, wglSwapLayerBuffers);
	CreateHook(OpenGL32, wglSwapLayerBuffers);
	Find(OpenGL32, wglGetProcAddress);
	CreateHook(OpenGL32, wglGetProcAddress);
	MH_EnableHook(MH_ALL_HOOKS);
}

void HookEngine()
{
	g_lpGameDLL = GetModuleHandleA("game.dll");

	void* handle;
	void* base;
	size_t size;

	if (MemUtils::GetModuleInfo(L"game.dll", &handle, &base, &size))
	{
		// TODO: fix these - keyboardcrash
		/*g_GetWindowXoffset = *(float*)(reinterpret_cast<HMODULE>(handle) + GETWINDOWXOFFSET_OFFSET);
		g_GetWindowYoffset = *(float*)(reinterpret_cast<HMODULE>(handle) + GETWINDOWYOFFSET_OFFSET*/

		auto utils = Utils::Utils(handle, base, size);

		MemUtils::AddSymbolLookupHook(handle, reinterpret_cast<void*>(ORIG_SetGameAreaFOV), 
									reinterpret_cast<void*>(HOOKED_SetGameAreaFOV));
		
		auto fSetGameAreaFOV = utils.FindAsync(ORIG_SetGameAreaFOV, patterns::engine::SetGameAreaFOV);
		auto patternSetGameAreaFOV = fSetGameAreaFOV.get();
		if (ORIG_SetGameAreaFOV)
		{
			int status;

			CreateHook(GameDLL, SetGameAreaFOV);
			MH_EnableHook(MH_ALL_HOOKS);
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_DETACH)
	{
		if (lpvReserved != nullptr) return TRUE;

		MH_Uninitialize();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		Discord_ClearPresence();
		Discord_Shutdown();
		FreeLibraryAndExitThread(hInstDll, NULL);
	}

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
#if _DEBUG
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		printf("Console allocated\n");
#endif

		DisableThreadLibraryCalls(hInstDll);

		MH_STATUS status = MH_Initialize();
		if (status != MH_OK)
		{
			printf("MH_Initialize failed: %s\n", MH_StatusToString(status));
			return FALSE;
		}

		HookOpenGL();
		HookEngine();
	}

    return TRUE;
}