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
		if (ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowFocused())
		{
			ImGui::OpenPopup("OptionsHyerarchy");
		}
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 3));
	if (ImGui::BeginPopup("OptionsHyerarchy"))
	{
		ShowOptions();
		ImGui::EndMenu();
	}

	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		ImGui::PushID(i);
		App->scene->gameobjects[i]->ShowHierarchy();
		ImGui::PopID();
	}
	ImGui::PopStyleVar();
	// Open Window To Delete GameObject
	if (toDelete != nullptr)
	{
		if (showconfirmDelete == false && dont_ask_me_next_time)
		{
			toDelete->SettoDelete();
			toDelete = nullptr;
		}
		else
		{
			static int width = 0;
			static int height = 0;
			bool active = true;
			//SDL_GetWindowSize(App->window->window, &width, &height);
			//ImGui::SetNextWindowPos(ImVec2(width / 2 - 180, height / 2 - 80));
			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("Delete GameObject", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &wait_toSelect);
			ImGui::PopStyleVar();
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				showconfirmDelete = !wait_toSelect;
				dont_ask_me_next_time = wait_toSelect;
				toDelete->SettoDelete();
				toDelete = nullptr;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				showconfirmDelete = !wait_toSelect;
				dont_ask_me_next_time = wait_toSelect;
				toDelete = nullptr;
			}
			if (active == false)
			{
				toDelete = nullptr;
			}
			ImGui::End();
		}
	}

	if (App->scene->drag && ImGui::IsMouseReleased(0))
		App->scene->drag = nullptr;

	EndDock();
}

void Hierarchy::ShowOptions()
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

void Hierarchy::SetGameObjecttoDelete(GameObject* todelete)
{
	toDelete = todelete;
}

void Hierarchy::ChangeShowConfirmDelete()
{
	dont_ask_me_next_time = !showconfirmDelete;
	wait_toSelect = false;
}


bool Hierarchy::CleanUp()
{
	return true;
}
