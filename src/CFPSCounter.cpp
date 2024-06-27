#include "CFPSCounter.h"

CFPSCounter::CFPSCounter() : CImguiMenu("CFPSCounter")
{

}

CFPSCounter::~CFPSCounter()
{

}

void CFPSCounter::Init()
{

}

void CFPSCounter::Draw() // TODO: migrate to in-game functions? (or even use UjAPI) - keyboardcrash
{
	ImGui::Begin("FPS Counter");

	ImGuiIO& io = ImGui::GetIO();

	ImGui::Text("%.1f fps", 1.0f / io.DeltaTime);

	ImGui::End();
}