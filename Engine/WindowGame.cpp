#include "WindowGame.h"
#include "Application.h"
#include "Scene.h"

WindowGame::WindowGame()
{
	active.push_back(Active());
	name = "WindowGame";
}


WindowGame::~WindowGame()
{
	active.clear();
}

bool WindowGame::Start()
{
	return true;
}

update_status WindowGame::Update(float dt)
{
	if (active[0].active)
	{
		ShowSceneWorld();
	}

	return UPDATE_CONTINUE;
}

void WindowGame::SetTexture()
{
	ImGui::Image((void*)App->scene->gameBuff->GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
}

void WindowGame::ShowSceneWorld()
{
	if (!BeginDock("Game", NULL, 0))
	{
		EndDock();
		return;
	}

	ImGui::Image((void*)App->scene->gameBuff->GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0)); // Set texture to buffer

	EndDock();
}

bool WindowGame::CleanUp()
{
	return false;
}
