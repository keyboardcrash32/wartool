#include "wartool.h"
#include "CImguiMgr.h"

_wglSwapBuffers ORIG_wglSwapBuffers = NULL;
HANDLE g_lpOpenGL32 = NULL;
CImguiMgr gImGui;
HWND gHwnd;

_wglSwapBuffers GetSwapBuffersAddr()
{
	return reinterpret_cast<_wglSwapBuffers>(GetProcAddress(LoadLibrary(TEXT("OpenGL32.dll")), "wglSwapBuffers"));
}

void HOOKED_wglSwapBuffers(HDC a1)
{
    static bool initialized = false;

    if (!initialized)
    {
        gHwnd = WindowFromDC(a1);
        gImGui.Init(gHwnd);

        initialized = true;
    }

    gImGui.Draw();
}

void HookOpenGL()
{
    g_lpOpenGL32 = GetModuleHandleA("opengl32.dll");
    if (!g_lpOpenGL32)
        return;

    ORIG_wglSwapBuffers = GetSwapBuffersAddr();
	
    int status;

    Find(OpenGL32, wglSwapBuffers);
	CreateHook(OpenGL32, wglSwapBuffers);
	MH_EnableHook(MH_ALL_HOOKS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        MH_Uninitialize();
        return FALSE;
    }

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        return FALSE;
    }

    HookOpenGL();

    return TRUE;
}