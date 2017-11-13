#include "WindowHierarchy.h"
#include "Application.h"
#include "Scene.h"
#include "GameObject.h"


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
	if (!BeginDock("Hierarchy", NULL, ImGuiWindowFlags_HorizontalScrollbar))
	{
		EndDock();
		return;
	}

	if (ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)))
	{
		if (ImGui::GetIO().MouseClicked[1])
		{
			ImGui::OpenPopup("OptionsHyerarchy");
		}
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 3));
	if (ImGui::BeginPopup("OptionsHyerarchy"))
	{
		ShowGameObjectOptions();
		ImGui::EndMenu();
	}
	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		ImGui::PushID(i);
		App->scene->gameobjects[i]->ShowHierarchy();
		ImGui::PopID();
	}
	ImGui::PopStyleVar();

	if (App->scene->drag && ImGui::IsMouseReleased(0))
		App->scene->drag = nullptr;

	EndDock();
}

void Hierarchy::ShowGameObjectOptions()
{
	//Create child Game Objects / Components
	ImGui::MenuItem("CREATE", NULL, false, false);
	if (ImGui::MenuItem("Empty"))
	{
		GameObject* empty = App->scene->CreateGameObject();
		App->gui->SetLinkInspector(empty);
	}
	if (ImGui::MenuItem("Cube"))
	{
		GameObject* cube = App->scene->CreateCube();
		App->gui->SetLinkInspector(cube);
	}
	if (ImGui::MenuItem("Sphere"))
	{
		GameObject* sphere = App->scene->CreateSphere();
		App->gui->SetLinkInspector(sphere);
	}
}


bool Hierarchy::CleanUp()
{
	return true;
}
