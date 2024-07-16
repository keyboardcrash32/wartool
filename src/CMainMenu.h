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

#ifndef CMAINMENU_H_INCLUDED
#define CMAINMENU_H_INCLUDED

#include "CImguiMenu.h"
#include <windows.h>

extern float g_CustomFovFix;

class CMainMenu : public CImguiMenu
{
public:
    CMainMenu();
    ~CMainMenu();

public:
    void Init() override;
    void Draw() override;

    bool lockCursor = false;
    bool PNGScreenshots = false;
    bool widescreenFix = false;

private:
    std::string m_gameBuildVersion;
};

#endif // CMAINMENU_H_INCLUDED