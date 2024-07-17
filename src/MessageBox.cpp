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

#include "MessageBox.h"

std::string WC3MessageBoxFormatType(int type)
{
    std::string str;

    switch (type)
    {
    case WC3MB_WARN:
        str = "WC3MB_WARN";
        break;
    case WC3MB_ERROR:
        str = "WC3MB_ERROR";
        break;
    case WC3MB_QUESTION:
        str = "WC3MB_QUESTION";
        break;
    default:
        str = "Unknown: " + type;
        break;
    }

    return str;
}

void __fastcall HOOKED_WC3MessageBox(int type, const char* string, int a3, int a4, int a5, int a6, int a7)
{
    // TODO: print wide string (since players can have different languages) - keyboardcrash
    printf("[GameDLL] WC3MessageBox (%s): %s\n", WC3MessageBoxFormatType(type).c_str(), string);

    ORIG_WC3MessageBox(type, string, a3, a4, a5, a6, a7);
}