#include "ModuleInspector.h"
#include "Application.h"



Inspector::Inspector() : WindowManager()
{
	active.push_back(Active());
}

Inspector::~Inspector()
{
	active.clear();
}

bool Inspector::Start()
{

	return true;
}

update_status Inspector::Update(float dt)
{
	if (active[0].active)
		ShowInspector();
	return UPDATE_CONTINUE;
}

void Inspector::ShowInspector()
{
	static int width;
	static int height;
	SDL_GetWindowSize(App->window->window, &width, &height);
	ImGui::SetNextWindowPos(ImVec2(width - 300, 20));
	ImGui::SetNextWindowSize(ImVec2(300, height - 20));
	if (!ImGui::Begin("Inspector", &App->gui->winManager[INSPECTOR]->active[0].active, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse)) //TODO ELLIOT CLOSE Windows example
	{
		ImGui::End();
		return;
	}
	ImGui::Text("HELLO WORLD");
	ImGui::End();
}

bool Inspector::CleanUp()
{

	return true;
}
