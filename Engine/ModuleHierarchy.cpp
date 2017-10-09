#include "ModuleHierarchy.h"
#include "Application.h"


Hierarchy::Hierarchy() : WindowManager()
{
	active.push_back(Active());
	name = "Hierarchy";
}

Hierarchy::~Hierarchy()
{
	active.clear();
}

bool Hierarchy::Start()
{

	return true;
}

update_status Hierarchy::Update(float dt)
{
	if (active[0].active)
		ShowHierarchy();
	return UPDATE_CONTINUE;
}

void Hierarchy::ShowHierarchy()
{
	static int width;
	static int height;
	SDL_GetWindowSize(App->window->window, &width, &height);
	//ImGui::SetNextWindowPos(ImVec2(width - 300, 20));
	//ImGui::SetNextWindowSize(ImVec2(300, height - 20 - (height - 700)));
	if (!BeginDock("Hierarchy", &App->gui->winManager[HIERARCHY]->active[0].active, ImGuiWindowFlags_HorizontalScrollbar)) //TODO ELLIOT CLOSE Windows example
	{
		EndDock();
		return;
	}
	ImGui::Columns(2);
	ImGui::Text("HELLO WORLD");

	EndDock();
}

bool Hierarchy::CleanUp()
{
	return true;
}
