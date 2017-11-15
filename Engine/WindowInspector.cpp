#include "Application.h"
#include "WindowInspector.h"
#include "GameObject.h"

Inspector::Inspector() : WindowManager()
{
	active.push_back(Active());
	name = "Inspector";
}

Inspector::~Inspector()
{
	active.clear();
	selected_object = nullptr;
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
	ImGui::SetNextWindowSize(ImVec2(300, height - 20 - (height - 700)));
	
	if (!BeginDock("Inspector", NULL, ImGuiWindowFlags_HorizontalScrollbar)) //TODO ELLIOT CLOSE Windows example
	{
		EndDock();
		return;
	}
	if (ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)))
	{
		if (ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowFocused())
		{
			ImGui::OpenPopup("OptionsInspector");
		}
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 3));
	if (ImGui::BeginPopup("OptionsInspector"))
	{
		ShowOptions();
		ImGui::EndMenu();
	}
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (selected_object != nullptr)
	{ 
		selected_object->ShowInspectorInfo();
	}
	ImGui::PopStyleVar(2);
	EndDock();
}

void Inspector::LinkObject(GameObject * obj)
{
	if (obj != nullptr)
	{
		selected_object = obj;
	}
}

void Inspector::SetLinkObjectNull()
{
	selected_object = nullptr;
}

GameObject* Inspector::GetSelected() const
{
	return selected_object;
}

void Inspector::ShowComponentsOptions()
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
}

void Inspector::ShowOptions()
{
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

bool Inspector::CleanUp()
{

	return true;
}
