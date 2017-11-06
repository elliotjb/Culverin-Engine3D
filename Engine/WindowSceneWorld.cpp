#include "WindowSceneWorld.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ImGui/ImGuizmo.h"

SceneWorld::SceneWorld() : WindowManager()
{
	active.push_back(Active());
	name = "SceneWorld";
}

SceneWorld::~SceneWorld()
{
	active.clear();
}

bool SceneWorld::Start()
{
	return true;
}

update_status SceneWorld::Update(float dt)
{
	if (active[0].active)
	{
		ShowSceneWorld();
	}

	return UPDATE_CONTINUE;
}

void SceneWorld::Draw()
{
	if (active[0].active)
	{
		ImGui::Image((void*)App->scene->sceneBuff->GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
	}
}

void SceneWorld::ShowSceneWorld()
{
	if (!BeginDock("Scene", NULL, 0))
	{
		EndDock();
		return;
	}

	// Generate mouse ray ---------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//App->camera->SetRay(App->input->GetMouseX(), App->input->GetMouseY());
		mouse_pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
		mouse_pos.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y;

		if (mouse_pos.x >= 0 && mouse_pos.y >= 0 &&
			mouse_pos.x <= ImGui::GetWindowWidth() && mouse_pos.y <= ImGui::GetWindowHeight())
		{
			App->camera->MousePick(mouse_pos.x, mouse_pos.y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			//LOG("MOUSE CLICK (%f, %f).", mouse_pos.x, mouse_pos.y);
		}
	}

	App->camera->CanMoveCamera = ImGui::IsMouseHoveringWindow(); //TODO ELLIOT CHange to variable in WindowManager.h
	
	ImGui::Image((void*)App->scene->sceneBuff->GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0)); // Set texture to buffer

	EndDock();
}

bool SceneWorld::CleanUp()
{
	return true;
}

void SceneWorld::GetWindowParams(float & x, float & y, float & w, float & h) const
{
	x = ImGui::GetWindowPos().x;
	y = ImGui::GetWindowPos().y;
	w = ImGui::GetWindowWidth();
	h = ImGui::GetWindowHeight();
}
