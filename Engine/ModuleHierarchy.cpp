#include "ModuleHierarchy.h"
#include "Application.h"
#include "_Sphere.h"


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
	/*for (int i = 0; i < App->geometry_manager->objects.size(); i++)
	{
		if (App->geometry_manager->objects[i]->type == P_SPHERE)
		{
			ImGui::PushID(i);
			if (ImGui::TreeNode("Sphere:"))
			{
				_Sphere* temp = (_Sphere*)App->geometry_manager->objects[i];
				ImGui::Spacing();
				ImGui::Text("Position:");
				ImGui::Text("x: %.3f", temp->pos.x);
				ImGui::SameLine();
				ImGui::Text("y: %.3f", temp->pos.y);
				ImGui::SameLine();
				ImGui::Text("z: %.3f", temp->pos.z);
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Radius: %.3f", temp->radius);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}*/


	EndDock();
}

bool Hierarchy::CleanUp()
{
	return true;
}
