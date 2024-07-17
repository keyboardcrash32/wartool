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

#include "CMainMenu.h"

CMainMenu::CMainMenu() : CImguiMenu("CMainMenu")
{

}

CMainMenu::~CMainMenu()
{

}

char* GetGameBuildVersion()
{
    DWORD  verHandle = 0;
    UINT   size = 0;
    LPBYTE lpBuffer = NULL;
    DWORD  verSize = GetFileVersionInfoSizeA("game.dll", &verHandle);

    if (verSize != NULL)
    {
        LPSTR verData = new char[verSize];

        if (GetFileVersionInfoA("game.dll", verHandle, verSize, verData))
        {
            if (VerQueryValueA(verData, "\\", (VOID FAR * FAR*) & lpBuffer, &size))
            {
                if (size)
                {
                    VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        char buffer[1024];
                        sprintf(buffer, "%d.%d.%d.%d",
                            (verInfo->dwFileVersionMS >> 16) & 0xffff,
                            (verInfo->dwFileVersionMS >> 0) & 0xffff,
                            (verInfo->dwFileVersionLS >> 16) & 0xffff,
                            (verInfo->dwFileVersionLS >> 0) & 0xffff
                        );

                        return buffer;
                    }
                }
            }
        }
        delete[] verData;
    }

    return const_cast<char*>("Unknown");
}

void CMainMenu::Init()
{
    m_gameBuildVersion = GetGameBuildVersion();
    printf("[GameDLL] Warcraft build version: %s\n", m_gameBuildVersion.c_str());
}

void CMainMenu::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::Begin("WarTool Main Menu");

    ImGui::Text("Warcraft build version: %s", m_gameBuildVersion.c_str());

    ImGui::Checkbox("Lock cursor in a window", &lockCursor);
    ImGui::Checkbox("Widescreen Fix", &widescreenFix);
    ImGui::SliderFloat("Custom FOV fix", &g_CustomFovFix, 0.1f, 5.0f);

    ImGui::Checkbox("Save screenshots in .png", &PNGScreenshots);

    ImGui::End();
    ImGui::PopStyleColor();
}