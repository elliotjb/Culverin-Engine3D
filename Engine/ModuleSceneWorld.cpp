#include "ModuleSceneWorld.h"
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
	if (!BeginDock("SceneWorld", &App->gui->winManager[HIERARCHY]->active[0].active, ImGuiWindowFlags_HorizontalScrollbar)) //TODO ELLIOT CLOSE Windows example
	{
		EndDock();
		return;
	}



	EndDock();
}

bool SceneWorld::CleanUp()
{
	return true;
}
