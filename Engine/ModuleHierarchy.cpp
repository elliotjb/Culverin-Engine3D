#include "ModuleHierarchy.h"
#include "Application.h"
#include "_Sphere.h"
#include "_Model.h"


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

	if (haveModel)
	{
		if (ImGui::TreeNodeEx(model_name.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
		}
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
