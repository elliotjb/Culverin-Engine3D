#include "ModuleProject.h"
#include "Application.h"


Project::Project()
{
	active.push_back(Active());
	name = "Project";
}


Project::~Project()
{
	active.clear();
}

bool Project::Start()
{
	return true;
}

update_status Project::Update(float dt)
{
	if (active[0].active)
		ShowProject();

	return UPDATE_CONTINUE;
}

bool Project::CleanUp()
{

	return true;
}

void Project::ShowProject()
{
	if (!BeginDock("Project", NULL, 0)) //TODO ELLIOT CLOSE Windows example
	{
		EndDock();
		return;
	}
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));
	if (ImGui::BeginChild(ImGui::GetID("Project"), ImVec2(ImGui::GetWindowWidth(), 17)))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 2));
		if (ImGui::Button("Select.."))
			ImGui::OpenPopup("select");
		ImGui::PopStyleVar(2);
		ImGui::SameLine();
		if (ImGui::BeginPopup("select"))
		{
			if (ImGui::Selectable("Create Folder..."))
			{

			}
			if (ImGui::Selectable("Import file"))
			{

			}
			ImGui::EndPopup();
		}
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

	EndDock();
}