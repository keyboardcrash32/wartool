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

#ifndef CIMGUIMENU_H_INCLUDED
#define CIMGUIMENU_H_INCLUDED

#include "ImGuiIncludes.h"
#include <stdio.h> // for printf
#include <string>

class CImguiMenu
{
public:
    CImguiMenu(std::string menuName);
    ~CImguiMenu();

public:
    virtual void Draw();

protected:
    std::string m_menuName = "Unknown";
};

#endif // CIMGUIMENU_H_INCLUDED