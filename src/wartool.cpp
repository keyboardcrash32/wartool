#include "wartool.h"
#include "CImguiMgr.h"
#include "CLockCursor.h"

_wglSwapLayerBuffers ORIG_wglSwapLayerBuffers = NULL;
_wglGetProcAddress ORIG_wglGetProcAddress = NULL;
_glClear ORIG_glClear = NULL;
_WndProc ORIG_WndProc = NULL;
void* g_lpOpenGL32;
CImguiMgr gImGui;
CLockCursor gLockCursor;
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

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_DETACH)
	{
		if (lpvReserved != nullptr) return TRUE;

		MH_Uninitialize();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
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
	}

    return TRUE;
}