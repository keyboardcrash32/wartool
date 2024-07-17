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

#include "CImguiMgr.h"

CImguiMgr::CImguiMgr()
{
    
}

CImguiMgr::~CImguiMgr()
{

}

void CImguiMgr::Init(HWND hWnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplWin32_Init(hWnd);

    ImGui::StyleColorsDark();

    mainMenu.Init();
}

void CImguiMgr::Draw()
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    if (g_bShowMainMenu)
        mainMenu.Draw();

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = io.WantCaptureMouse;

    m_drawCalled = true;
}

void CImguiMgr::End()
{
    if (m_drawCalled) // just to prevent annoying errors
    {
        ImGui::EndFrame();

        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_drawCalled = false;
    }
}