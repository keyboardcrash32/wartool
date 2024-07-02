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

#ifndef MESSAGEBOX_H_INCLUDED
#define MESSAGEBOX_H_INCLUDED

#include <string>

#define WC3MB_WARN 0x00
#define WC3MB_ERROR 0x01
#define WC3MB_QUESTION 0x02

typedef void(__fastcall* _WC3MessageBox)(int type, const char* string, int a3, int a4, int a5, int a6, int a7);
extern _WC3MessageBox ORIG_WC3MessageBox;

std::string WC3MessageBoxFormatType(int type);
void __fastcall HOOKED_WC3MessageBox(int type, const char* string, int a3, int a4, int a5, int a6, int a7);

#define MsgBox(string, type) HOOKED_WC3MessageBox(type, string, 0, 0, 0, 4, 1);

#endif // MESSAGEBOX_H_INCLUDED