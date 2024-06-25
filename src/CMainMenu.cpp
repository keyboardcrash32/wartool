#include "CMainMenu.h"

CMainMenu::CMainMenu() : CImguiMenu("CMainMenu")
{

}

CMainMenu::~CMainMenu()
{

}

void CMainMenu::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::Begin("WarTool Main Menu");

	// TODO: add something useful - keyboardcrash

	ImGui::End();
    ImGui::PopStyleColor();
}