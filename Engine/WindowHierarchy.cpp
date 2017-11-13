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
			ImGui::OpenPopup("Test_1");
		}
	}
	if (ImGui::BeginPopup("Test_1"))
	{
		ShowGameObjectOptions();
		ImGui::EndMenu();
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		ImGui::CloseCurrentPopup();
	}
	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		ImGui::PushID(i);
		App->scene->gameobjects[i]->ShowHierarchy();
		ImGui::PopID();
	}

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
		//GameObject* empty = App->scene->CreateGameObject(this);
		//((Inspector*)App->gui->winManager[INSPECTOR])->LinkObject(empty);
	}
	if (ImGui::MenuItem("Cube"))
	{
		//GameObject* cube = App->scene->CreateCube(this);
		//((Inspector*)App->gui->winManager[INSPECTOR])->LinkObject(cube);
	}
	if (ImGui::MenuItem("Sphere"))
	{
		//GameObject* sphere = App->scene->CreateSphere(this);
		//((Inspector*)App->gui->winManager[INSPECTOR])->LinkObject(sphere);
	}
	ImGui::MenuItem("ADD COMPONENT", NULL, false, false);
	if (ImGui::MenuItem("Transform"))
	{
		//AddComponent(Comp_Type::C_TRANSFORM);
	}
	if (ImGui::MenuItem("Mesh"))
	{
		//AddComponent(Comp_Type::C_MESH);
	}
	if (ImGui::MenuItem("Material"))
	{
		//AddComponent(Comp_Type::C_MATERIAL);
	}
}


bool Hierarchy::CleanUp()
{
	return true;
}
