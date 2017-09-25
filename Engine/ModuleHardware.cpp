#include "ModuleHardware.h"
#include "Application.h"
#include "SDL\include\SDL.h"

Hardware::Hardware(bool start_enabled) : WindowManager(start_enabled)
{

}

Hardware::~Hardware()
{
}

bool Hardware::Start()
{
	return true;
}

update_status Hardware::Update(float dt)
{
	if (active)
	{
		ImGui::Begin("Hardware");

		ImGui::Text("SDL Version: ");
		SDL_version compiled;
		SDL_VERSION(&compiled);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);
		//ImGui::SameLine(); ShowHelpMarker("The TextDisabled color is stored in ImGuiStyle.");
		ImGui::Separator();
		ImGui::Text("CPUs: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", SDL_GetCPUCount());
		ImGui::Text("System RAM: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d Mb (Cache: %d Kb)", SDL_GetSystemRAM(), SDL_GetCPUCacheLineSize());
		ImGui::Separator();
		ImGui::Text("Vendor: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));
		ImGui::Text("GPU: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_RENDERER));
		ImGui::Text("Brand: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));
		ImGui::Text("Version: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VERSION));

		ImGui::End();
	}


	return UPDATE_CONTINUE;
}

bool Hardware::CleanUp()
{

	return true;
}

//void Hardware::OpenClose()
//{
//	hardware_activated = !hardware_activated;
//}

//bool Hardware::IsOpen()
//{
//	return hardware_activated;
//}
