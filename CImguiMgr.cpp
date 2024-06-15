#include "CImguiMgr.h"

CImguiMgr::CImguiMgr(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui_ImplOpenGL2_Init();
	ImGui_ImplWin32_Init(hWnd);
}

CImguiMgr::~CImguiMgr()
{

}

void CImguiMgr::Draw()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplOpenGL2_NewFrame();
	ImGui::NewFrame();

	// TODO: actually draw smth - keyboardcrash

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}