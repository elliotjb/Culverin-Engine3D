#include "WindowSceneWorld.h"
#include "Application.h"

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
		ShowSceneWorld();
	return UPDATE_CONTINUE;
}

void SceneWorld::ShowSceneWorld()
{
	if (!BeginDock("Scene", NULL, 0))
	{
		EndDock();
		return;
	}
	App->camera->CanMoveCamera = ImGui::IsMouseHoveringWindow(); //TODO ELLIOT CHange to variable in WindowManager.h
	
	ImGui::Image((void*)App->scene->frBuff->GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	EndDock();
}

bool SceneWorld::CleanUp()
{
	return true;
}
