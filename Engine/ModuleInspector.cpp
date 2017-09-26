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
	ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() - 300, 20));
	ImGui::SetNextWindowSize(ImVec2(300, App->window->GetHeight()));
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
