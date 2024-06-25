#include "CImguiMenu.h"

CImguiMenu::CImguiMenu(std::string menuName) : m_menuName(menuName)
{
	printf("%s Imgui Menu created\n", m_menuName.c_str());
}

CImguiMenu::~CImguiMenu()
{
	printf("%s Imgui Menu deleted\n", m_menuName.c_str());
}

void CImguiMenu::Draw()
{

}