#include "ModuleHierarchy.h"
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

	for (uint i = 0; i < App->scene->gameobjects.size(); i++)
	{
		App->scene->gameobjects[i]->ShowHierarchy();
	}

	EndDock();
}

void Hierarchy::SetName(_Model* model)
{
	if (model != nullptr)
	{
		haveModel = true;
		model_name = model->base_info.name;
	}
}

bool Hierarchy::CleanUp()
{
	return true;
}
