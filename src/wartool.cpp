#include "wartool.h"
#include "CImguiMgr.h"

_wglSwapLayerBuffers ORIG_wglSwapLayerBuffers = NULL;
_wglGetProcAddress ORIG_wglGetProcAddress = NULL;
_glClear ORIG_glClear = NULL;
_WndProc ORIG_WndProc = NULL;
void* g_lpOpenGL32;
CImguiMgr gImGui;
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

        initialized = true;
    }

    gImGui.End();

    return ORIG_wglSwapLayerBuffers(a1, a2);
}

void __stdcall HOOKED_glClear(GLbitfield a1)
{
	static bool initialized = false;

	gImGui.Draw();

    ORIG_glClear(a1);
}

PROC __stdcall HOOKED_wglGetProcAddress(LPCSTR a1)
{
    printf("HOOKED_wglGetProcAddress: %s\n", a1);
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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        MH_Uninitialize();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

        return TRUE;
    }

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
#if 1
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        printf("Console allocated\n");
#endif

        DisableThreadLibraryCalls(hModule);

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