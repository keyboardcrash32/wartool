#include "wartool.h"
#include "CImguiMgr.h"

_wglSwapLayerBuffers ORIG_wglSwapLayerBuffers = NULL;
void* g_lpOpenGL32;
CImguiMgr gImGui;
HWND gHwnd;

_wglSwapLayerBuffers GetSwapLayerBuffersAddr()
{
	return reinterpret_cast<_wglSwapLayerBuffers>(GetProcAddress(LoadLibrary(TEXT("OpenGL32.dll")), "wglSwapLayerBuffers"));
}

int __stdcall HOOKED_wglSwapLayerBuffers(HDC a1, UINT a2)
{
    static bool initialized = false;

    if (!initialized)
    {
        gHwnd = WindowFromDC(a1);
        gImGui.Init(gHwnd);

        initialized = true;
    }

    gImGui.Draw(); // TODO: fix black screen - keyboardcrash

    return ORIG_wglSwapLayerBuffers(a1, a2);
}

void HookOpenGL()
{
    g_lpOpenGL32 = GetModuleHandleA("opengl32.dll");
    if (!g_lpOpenGL32)
        return;

    ORIG_wglSwapLayerBuffers = GetSwapLayerBuffersAddr();
	
    int status;

    Find(OpenGL32, wglSwapLayerBuffers);
	CreateHook(OpenGL32, wglSwapLayerBuffers);
	MH_EnableHook(MH_ALL_HOOKS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        MH_Uninitialize();
        return FALSE;
    }

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
#if 0
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
#endif

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