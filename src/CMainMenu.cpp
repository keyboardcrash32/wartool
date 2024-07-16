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
    printf("Warcraft build version: %s\n", m_gameBuildVersion.c_str());
}

void CMainMenu::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::Begin("WarTool Main Menu");

    ImGui::Text("Warcraft build version: %s", m_gameBuildVersion.c_str());

    ImGui::Checkbox("Lock cursor in a window", &lockCursor);
    ImGui::Checkbox("Widescreen Fix", &widescreenFix);
    ImGui::SliderFloat("Custom FOV fix", &g_CustomFovFix, 0.1f, 5.0f);

    ImGui::Checkbox("Save screenshots in .png (NOT IMPLEMENTED)", &PNGScreenshots);

    ImGui::End();
    ImGui::PopStyleColor();
}