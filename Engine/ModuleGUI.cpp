#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "imgui.h"
#include "Gl3W\include\glew.h"
#include "imgui_impl_sdl_gl3.h"

ModuleGUI::ModuleGUI(Application * app, bool start_enabled): Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Start()
{
	return true;
}

update_status ModuleGUI::Update(float dt)
{
	//IMGUI----------------------------------------------------
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	ImGui::ShowTestWindow();


	// CLOSE APP WINDOW --------
	ImGui::Begin("CLOSE APP");
	static int clicked = 0;
	if (ImGui::Button("CLICK HERE"))
		clicked++;
	if (clicked & 1)
	{
		return UPDATE_STOP;
	}
	ImGui::End();
	// --------------------------

	if (App->console->IsOpen())
	{
		ShowExampleAppConsole();
	}

	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	return true;
}

void ModuleGUI::ShowExampleAppConsole()
{
	App->console->Draw("CONSOLE");
}
