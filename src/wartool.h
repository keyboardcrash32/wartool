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

typedef int(__stdcall* _wglSwapLayerBuffers)(HDC, UINT);
typedef PROC(__stdcall* _wglGetProcAddress)(LPCSTR);
typedef void(__stdcall* _glClear)(GLbitfield);
typedef LRESULT(APIENTRY* _WndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define CreateHook(lib, func_name) \
        status = MH_CreateHook(ORIG_##func_name, HOOKED_##func_name, reinterpret_cast<void**>(&ORIG_##func_name)); \
        if (status != MH_OK) { \
            printf("[" #lib "] Couldn't create hook for " #func_name ".\n"); \
        }

#define Find(lib, func_name) \
    if ((ORIG_##func_name = reinterpret_cast<_##func_name>(GetProcAddress(reinterpret_cast<HMODULE>(g_lp##lib), #func_name)))) \
        printf("[game dll] Found " #func_name " at %p.\n", ORIG_##func_name); \
    else \
        printf("[game dll] Could not find " #func_name ".\n");

#endif // WARTOOL_H_INCLUDED