/*
    Copyright (C) 2024 keyboardcrash

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "wartool.h"
#include "CImguiMgr.h"
#include "CDiscordRPC.h"
#include "MessageBox.h"

// OpenGL
_wglSwapLayerBuffers ORIG_wglSwapLayerBuffers = NULL;
_wglGetProcAddress ORIG_wglGetProcAddress = NULL;
_glClear ORIG_glClear = NULL;
_WndProc ORIG_WndProc = NULL;

// Engine
_SetGameAreaFOV ORIG_SetGameAreaFOV = NULL;
_WC3MessageBox ORIG_WC3MessageBox = NULL;
_OnPostGameStart ORIG_OnPostGameStart = NULL;
_OnPostPlayerLeave ORIG_OnPostPlayerLeave = NULL;
_TakeScreenshot ORIG_TakeScreenshot = NULL;
float g_GetWindowXoffset;
float g_GetWindowYoffset;
bool g_WidescreenFix = false;
float g_CustomFovFix = 1.0f;
bool g_bInGoingGame = false;

bool g_bInitialized = false;
bool g_bShowMainMenu = false;

void* g_lpOpenGL32;
void* g_lpGameDLL;
void* g_lpDirect3D;
CImguiMgr gImGui;
CDiscordRPC gDiscordRPC;
HWND gHwnd;

void TakeScreenshot();

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
inline long __stdcall HOOKED_WndProc(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

    if (msg == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_INSERT:
            g_bShowMainMenu = !g_bShowMainMenu;
            break;

        case VK_HOME:
            TakeScreenshot();
            break;
        }
    }

    return CallWindowProcA(ORIG_WndProc, hWnd, msg, wParam, lParam);
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
    static bool cursorLocked = false;
    RECT windowRect, oldRect;

    if (!g_bInitialized)
    {
        gHwnd = WindowFromDC(a1);

        ORIG_WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(gHwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HOOKED_WndProc)));

        gImGui.Init(gHwnd);

        gDiscordRPC.Init();
        gDiscordRPC.SetGameState(INMENU);

        g_bInitialized = true;
    }

    gImGui.End();

    if (gImGui.mainMenu.lockCursor && !cursorLocked && g_bInGoingGame)
    {
        GetClipCursor(&oldRect); // for unclipping
        GetClientRect(gHwnd, &windowRect);

        MapWindowPoints(gHwnd, nullptr, reinterpret_cast<POINT*>(&windowRect), 2);

        ClipCursor(&windowRect);
        cursorLocked = true;
    }

    if (!gImGui.mainMenu.lockCursor && cursorLocked)
    {
        ClipCursor(&oldRect);
        cursorLocked = false;
    }

    g_WidescreenFix = gImGui.mainMenu.widescreenFix;

    return ORIG_wglSwapLayerBuffers(a1, a2);
}

void __stdcall HOOKED_glClear(GLbitfield a1)
{
    gImGui.Draw();

    ORIG_glClear(a1);
}

PROC __stdcall HOOKED_wglGetProcAddress(LPCSTR a1) // just for logging
{
    printf("[OpenGL32] wglGetProcAddress: %s\n", a1);
    return ORIG_wglGetProcAddress(a1);
}

int __fastcall HOOKED_SetGameAreaFOV(Matrix1* a1, int a2, float a3, float a4, float a5, float a6)
{
    if (!g_WidescreenFix)
        return ORIG_SetGameAreaFOV(a1, a2, a3, a4, a5, a6);

    // TODO: find a way to read these values from any version of the game - keyboardcrash
    g_GetWindowXoffset = *(float*)MakePtr(g_lpGameDLL, GETWINDOWXOFFSET_OFFSET);
    g_GetWindowYoffset = *(float*)MakePtr(g_lpGameDLL, GETWINDOWYOFFSET_OFFSET);

    if (g_GetWindowXoffset == 0 || g_GetWindowYoffset == 0)
        return ORIG_SetGameAreaFOV(a1, a2, a3, a4, a5, a6);

    float ScreenX = g_GetWindowXoffset;
    float ScreenY = g_GetWindowYoffset;

    float v1 = 1.0f / sqrt(a4 * a4 + 1.0f);
    float v2 = tan(v1 * a3 * 0.5f);

    float v3 = v2 * a5;
    float v4 = v3 * a4;

    a1->flt1 = ((a5 * (4.0f / 3.0f)) / (ScreenX / ScreenY) * g_CustomFovFix) / v4; // Fix 4:3 to WindowX/WindowY
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

    return 0;
}

int __fastcall HOOKED_OnPostGameStart(int a1)
{
    gDiscordRPC.SetGameState(INGAME);
    g_bInGoingGame = true;

    return ORIG_OnPostGameStart(a1);
}

int __fastcall HOOKED_OnPostPlayerLeave(int a1)
{
    gDiscordRPC.SetGameState(INMENU);
    g_bInGoingGame = false;

    return ORIG_OnPostPlayerLeave(a1);
}

int __fastcall HOOKED_TakeScreenshot(int a1, char* a2, int a3, DWORD a4, int a5, float a6,
                                    int a7, int a8, int a9, int a10, float a11, float a12)
{
    if (!gImGui.mainMenu.PNGScreenshots)
        return ORIG_TakeScreenshot(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);

    return 0;
}

void TakeScreenshot() // TODO: fix - keyboardcrash
{
    if (!gImGui.mainMenu.PNGScreenshots)
        return;

    RECT windowRect;
    if (!GetClientRect(gHwnd, &windowRect))
    {
        printf("Cannot get window rectangle (GetClientRect failure)\n");
        return;
    }

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    uint8_t* pixels = new uint8_t[windowWidth * windowHeight * RGB_COLORS];

    HDC hScreenDC = GetDC(gHwnd);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = GetDeviceCaps(hScreenDC, HORZRES);
    bmi.bmiHeader.biHeight = -GetDeviceCaps(hScreenDC, VERTRES);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    HBITMAP hBitmap = CreateDIBSection(hMemoryDC, &bmi, DIB_RGB_COLORS, (LPVOID*)&pixels, NULL, 0);

    HGDIOBJ hOldBitmap = SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, bmi.bmiHeader.biWidth, -bmi.bmiHeader.biHeight, hScreenDC, 0, 0, SRCCOPY);

    int lineSize = windowWidth * RGB_COLORS;
    uint8_t* line = new uint8_t[lineSize];

    for (int y = 0; y < windowHeight / 2; y++)
    {
        int topOffset = y * lineSize;
        int bottomOffset = (windowHeight - y - 1) * lineSize;
        memcpy(line, &pixels[topOffset], lineSize);
        memcpy(&pixels[topOffset], &pixels[bottomOffset], lineSize);
        memcpy(&pixels[bottomOffset], line, lineSize);
    }

    std::string filename = "scrnsht.png";
    int result = stbi_write_png(filename.c_str(), windowWidth, windowHeight, RGB_COLORS, pixels, windowWidth * RGB_COLORS);
    if (result == 1)
    {
        printf("Took a screenshot %s\n", filename.c_str());
    }
    else
    {
        printf("Failed to take a screenshot %s (%d)\n", filename.c_str(), result);
    }

    SelectObject(hMemoryDC, hOldBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(gHwnd, hScreenDC);
    DeleteObject(hBitmap);
    //delete[] pixels;
    delete[] line;
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
        auto utils = Utils::Utils(handle, base, size);

        MemUtils::AddSymbolLookupHook(handle, reinterpret_cast<void*>(ORIG_SetGameAreaFOV), 
                                    reinterpret_cast<void*>(HOOKED_SetGameAreaFOV));

        MemUtils::AddSymbolLookupHook(handle, reinterpret_cast<void*>(ORIG_WC3MessageBox),
                                    reinterpret_cast<void*>(HOOKED_WC3MessageBox));

        MemUtils::AddSymbolLookupHook(handle, reinterpret_cast<void*>(ORIG_OnPostGameStart),
                                    reinterpret_cast<void*>(HOOKED_OnPostGameStart));

        MemUtils::AddSymbolLookupHook(handle, reinterpret_cast<void*>(ORIG_OnPostPlayerLeave),
                                    reinterpret_cast<void*>(HOOKED_OnPostPlayerLeave));

        MemUtils::AddSymbolLookupHook(handle, reinterpret_cast<void*>(ORIG_TakeScreenshot),
                                    reinterpret_cast<void*>(HOOKED_TakeScreenshot));

        int status;

        SPTFind(SetGameAreaFOV);
        CreateHook(GameDLL, SetGameAreaFOV);
        SPTFind(WC3MessageBox);
        CreateHook(GameDLL, WC3MessageBox);
        SPTFind(OnPostGameStart);
        CreateHook(GameDLL, OnPostGameStart);
        SPTFind(OnPostPlayerLeave);
        CreateHook(GameDLL, OnPostPlayerLeave);
        SPTFind(TakeScreenshot);
        CreateHook(GameDLL, TakeScreenshot);

        MH_EnableHook(MH_ALL_HOOKS);
    }
}

void CheckForDirect3D()
{
    HANDLE d3d8 = GetModuleHandleA("d3d8.dll");
    if (d3d8)
    {
        MessageBoxA(GetForegroundWindow(),
            "It looks like you've launched the game in Direct3D mode.\n"
            "Note, that WarTool does not support Direct3D.\n"""
            "Please migrate to OpenGL by running the\n"
            "game executable with -opengl flag.\n"
            , "WarTool error!", MB_ICONERROR);

        ExitProcess(EXIT_FAILURE);
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
        SetConsoleTitleA("WarTool Debug Console");
        freopen("CONOUT$", "w", stdout);
        printf("Console allocated\n");
#endif

        CheckForDirect3D();

        DisableThreadLibraryCalls(hInstDll);

        MH_STATUS status = MH_Initialize();
        if (status != MH_OK)
        {
            printf("MH_Initialize failed: %s\n", MH_StatusToString(status));
            return FALSE;
        }

        HookEngine();
        HookOpenGL();
    }

    return TRUE;
}