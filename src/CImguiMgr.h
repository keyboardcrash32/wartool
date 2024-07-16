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

#ifndef CIMGUIMGR_H_INCLUDED
#define CIMGUIMGR_H_INCLUDED

#include <windows.h>
#include <gl/GL.h>
#include "ImGuiIncludes.h"
#include "CMainMenu.h"

extern bool g_bShowMainMenu;

class CImguiMgr
{
public:
    CImguiMgr();
    ~CImguiMgr();

public:
    void Init(HWND hWnd);
    void Draw();
    void End();

    CMainMenu mainMenu;
    bool showMainMenu = false;

private:
    bool m_drawCalled = false;
};

#endif // CIMGUIMGR_H_INCLUDED