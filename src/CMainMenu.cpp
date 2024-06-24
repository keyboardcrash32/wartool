﻿#include "CMainMenu.h"

CMainMenu::CMainMenu() : CImguiMenu("CMainMenu")
{

}

CMainMenu::~CMainMenu()
{

}

void CMainMenu::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::Begin("Main Menu");
	ImGui::Text(R"(
       .....           .....
   ,ad8PPPP88b,     ,d88PPPP8ba,
  d8P"      "Y8b, ,d8P"      "Y8b
 dP'           "8a8"           `Yd
 8(              "              )8
 I8                             8I
  Yb,                         ,dP
   "8a,                     ,a8"
     "8a,                 ,a8"
       "Yba             adP"
         `Y8a         a8P'
           `88,     ,88'
             "8b   d8"
              "8b d8"
               `888'
                 "
				)");
	ImGui::End();
    ImGui::PopStyleColor();
}