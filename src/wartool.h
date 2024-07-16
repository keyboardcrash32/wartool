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

#ifndef WARTOOL_H_INCLUDED
#define WARTOOL_H_INCLUDED

#include <windows.h>
#include "MinHook.h"
#include <stdio.h>
#include <gl/gl.h>
#include "Utils.hpp"
#include "wc3_patterns.hpp" // must be included after Utils.hpp

// OpenGL
typedef int(__stdcall* _wglSwapLayerBuffers)(HDC, UINT);
typedef PROC(__stdcall* _wglGetProcAddress)(LPCSTR);
typedef void(__stdcall* _glClear)(GLbitfield);
typedef LRESULT(APIENTRY* _WndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Engine
#pragma pack(push, 1)
struct Matrix1 // Matrix 4x4
{
    float flt1;  // 0
    float flt2;  // 4
    float flt3;  // 8
    float flt4;  // 12
    float flt5;  // 16
    float flt6;  // 20
    float flt7;  // 24
    float flt8;  // 28
    float flt9;  // 32
    float flt10; // 36
    float flt11; // 40
    float flt12; // 44
    float flt13; // 48
    float flt14; // 52
    float flt15; // 56
    float flt16; // 60
};
#pragma pack(pop, 1)

#define GETWINDOWXOFFSET_OFFSET 0xADE91C
#define GETWINDOWYOFFSET_OFFSET 0xADE918

typedef int(__fastcall* _SetGameAreaFOV)(Matrix1* a1, int a2, float a3, float a4, float a5, float a6);
typedef int(__fastcall* _OnPostGameStart)(int a1);
typedef int(__fastcall* _OnPostPlayerLeave)(int a1);

// Discord RPC
#define PLAYERSINGAME_OFFSET 0xAB4E08
#define MAPNAME_OFFSET 0xAAE7C0

#define CreateHook(lib, func_name) \
        status = MH_CreateHook(ORIG_##func_name, HOOKED_##func_name, reinterpret_cast<void**>(&ORIG_##func_name)); \
        if (status != MH_OK) { \
            printf("[" #lib "] Couldn't create hook for " #func_name ".\n"); \
        }

#define Find(lib, func_name) \
    if ((ORIG_##func_name = reinterpret_cast<_##func_name>(GetProcAddress(reinterpret_cast<HMODULE>(g_lp##lib), #func_name)))) \
        printf("[" #lib "] Found " #func_name " at %p.\n", ORIG_##func_name); \
    else \
        printf("[" #lib "] Could not find " #func_name ".\n");


#define SPTFind(future_name)                                                                                                               \
    {                                                                                                                                      \
        auto f##future_name = utils.FindAsync(ORIG_##future_name, patterns::engine::future_name);                                          \
        auto pattern = f##future_name.get();                                                                                               \
        if (ORIG_##future_name)                                                                                                            \
        {                                                                                                                                  \
            printf("[GameDLL] Found " #future_name " at %p (using the %s pattern).\n", ORIG_##future_name, pattern->name()); \
        }                                                                                                                                  \
    }

#define MakePtr(a, b) (DWORD)((DWORD)a + (DWORD)b)

#endif // WARTOOL_H_INCLUDED