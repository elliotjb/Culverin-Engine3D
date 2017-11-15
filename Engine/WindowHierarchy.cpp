#include "WindowHierarchy.h"
#include "Application.h"
#include "Scene.h"
#include "GameObject.h"
#include "WindowInspector.h"

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
		if (ImGui::GetIO().MouseClicked[1] && ImGui::IsMouseHoveringWindow())
		{
			ImGui::OpenPopup("OptionsHyerarchy");
		}
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 3));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 6));
	if (ImGui::BeginPopup("OptionsHyerarchy"))
	{
		selected = ((Inspector*)App->gui->winManager[INSPECTOR])->GetSelected();
		ShowOptions();
		ImGui::EndMenu();
	}
	ImGui::PopStyleVar();
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
	if (ImGui::MenuItem("Copy"))
	{
		if (selected != nullptr)
		{
			copy = selected;
		}
	}
	if (ImGui::MenuItem("Paste"))
	{
		if (selected != nullptr && copy != nullptr)
		{
			if (selected->HaveParent())
			{
				// TODO ELLIOT
				// selected->GetParent()->AddChildGameObject_Copy(copie);
				// Copied = nullptr;
			}
			else
			{
				// App->scene->gameobjects.push_back(GameObject Copied);
				// Copied = nullptr;
			}
		}
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Rename", NULL, false, false))
	{
		// Not functional
	}
	if (ImGui::MenuItem("Duplicate", NULL, false, false))
	{
		if (selected != nullptr)
		{
			// selected->GetParent()->AddChildGameObject_Copy(selected);
		}
	}
	if (ImGui::MenuItem("Delete"))
	{
		if (selected != nullptr)
		{
			SetGameObjecttoDelete(selected);
		}
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Create Empty"))
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

void Hierarchy::SetGameObjectCopy(GameObject* copy_)
{
	if (copy_ != nullptr)
	{
		copy = copy_;
	}
}

const GameObject * Hierarchy::GetCopied() const
{
	return copy;
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
