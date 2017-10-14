#include "ModuleInspector.h"
#include "_Model.h"
#include "Application.h"



Inspector::Inspector() : WindowManager()
{
	active.push_back(Active());
	name = "Inspector";
}

Inspector::~Inspector()
{
	active.clear();
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

	if (model_loaded)
	{
		ShowModelInfo();
	}

	EndDock();
}

void Inspector::SetInfo(_Model* model)
{
	if (model != nullptr)
	{
		model_ref = model;

		std::string temp = model_ref->base_info.name.c_str();
		model_name = new char[temp.size() + 1];
		std::copy(temp.begin(), temp.end(), model_name);
		model_name[temp.size()] = '\0';
	}
}

void Inspector::SetTexInfo(_Model* model)
{
	model_ref = model;
}

void Inspector::ShowModelInfo() const
{
	//ImGui::Text("MODEL INFO");
	//ImGuiStyleVar_FramePadding
	//ImGuiInputTextFlags_ReadOnly
	//ImGui::PushStyleVar()
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth(), 40));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));
	if (ImGui::BeginChild(ImGui::GetID("Inspector"),ImVec2(ImGui::GetWindowWidth(), 40)))
	{
		static GLuint icon_GameObject = App->textures->LoadTexture("Images/UI/icon_GameObject.png");
		ImGui::Spacing();
		//ImGui
		ImGui::Text(""); ImGui::SameLine(5); 
		ImGui::Image((ImTextureID*)icon_GameObject, ImVec2(23, 23), ImVec2(-1, 1), ImVec2(0, 0)); ImGui::SameLine();
		static bool isEnable = true;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
		ImGui::Checkbox("##1", &isEnable);	ImGui::SameLine();
		ImGui::PopStyleVar();
		ImGui::InputText("", model_name, IM_ARRAYSIZE(model_name));
		ImGui::SameLine(); App->ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	ImGui::PopStyleColor();
	if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 pos = model_ref->base_info.position; //70
		float3 rot = model_ref->base_info.rotation; //70
		float3 scale = model_ref->base_info.scale; //70
		int op = ImGui::GetWindowWidth() / 4;
		ImGui::PushItemWidth(op - 40);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 1));
		ImGui::Text("Position"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("", &pos.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("", &pos.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("", &pos.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_pos.x, object_pos.y, object_pos.z);
		//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_rot.x, object_rot.y, object_rot.z);
		//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_scale.x, object_scale.y, object_scale.z);
		ImGui::Text("Rotation"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("", &rot.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("", &rot.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("", &rot.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Scale"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("", &scale.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("", &scale.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("", &scale.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopStyleVar();
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNodeEx("Total Geometry", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Total Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_vertex);
		ImGui::Text("Total Meshes:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_meshes);
		ImGui::Text("Total Faces:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_faces);
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("MESHES"))
	{
		for (uint i = 0; i < model_ref->meshes.size(); i++)
		{
			ImGui::Text(" MESH %i", i);
			ImGui::Text(" Vertices:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].vertices.size());
			ImGui::Text(" Faces:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].numFaces);

			ImGui::Separator();
		}
	}

	//if (ImGui::TreeNodeEx("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	ImGui::Text("Texture:");
	//	ImGui::Image((ImTextureID*)tex_id, ImVec2(150, 150), ImVec2(-1, 1), ImVec2(0, 0));
	//	ImGui::TreePop();
	//}
}

bool Inspector::CleanUp()
{

	return true;
}
