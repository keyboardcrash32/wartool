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
}

void CMainMenu::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::Begin("WarTool Main Menu");

	char* GameBuildVersion = GetGameBuildVersion();
	ImGui::Text("Warcraft build version: %s", GameBuildVersion);

	ImGui::End();
    ImGui::PopStyleColor();
}