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

	ImGui_ImplOpenGL2_Init();
	ImGui_ImplWin32_Init(hWnd);

	//ImGui::StyleColorsDark();
}

void CImguiMgr::Draw()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplOpenGL2_NewFrame();
	ImGui::NewFrame();

	// TODO: actually draw smth useful - keyboardcrash
	//ImGui::Begin("swag window");
	//ImGui::Button("dayyymmm");
	//ImGui::End();

	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}