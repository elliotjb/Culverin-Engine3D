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

void Inspector::ShowModelInfo() 
{
	//ImGui::Text("MODEL INFO");
	//ImGuiStyleVar_FramePadding
	//ImGuiInputTextFlags_ReadOnly
	//ImGui::PushStyleVar()
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth(), 40));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));
	if (ImGui::BeginChild(ImGui::GetID("Inspector"), ImVec2(ImGui::GetWindowWidth(), 40)))
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
		ImGui::InputText("##nameModel", model_name, IM_ARRAYSIZE(model_name));
		ImGui::SameLine(); App->ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");
		ImGui::PopStyleVar();
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();
		ImGui::Separator();
		float3 pos = model_ref->base_info.position; //70
		float3 rot = model_ref->base_info.rotation; //70
		float3 scale = model_ref->base_info.scale; //70
		int op = ImGui::GetWindowWidth() / 4;
		ImGui::PushItemWidth(op - 40);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 1));
		ImGui::Text("Position"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_1", &pos.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_1", &pos.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_1", &pos.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_pos.x, object_pos.y, object_pos.z);
		//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_rot.x, object_rot.y, object_rot.z);
		//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_scale.x, object_scale.y, object_scale.z);
		ImGui::Text("Rotation"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_2", &rot.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_2", &rot.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_2", &rot.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Scale"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_3", &scale.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_3", &scale.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_3", &scale.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
	ImGui::Separator();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Total Geometry", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();
		ImGui::Separator();
		ImGui::Text("Total Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_vertex);
		ImGui::Text("Total Meshes:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_meshes);
		ImGui::Text("Total Faces:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_faces);
		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
	ImGui::Separator();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Meshes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Separator();
		ImGui::PopStyleColor();
		for (uint i = 0; i < model_ref->meshes.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::Text("Mesh %i: ", i); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%s", model_ref->meshes[i].name.c_str());
			ImGui::Text("Vertices:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].vertices.size());
			ImGui::Text("Faces:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].numFaces);
			ImGui::Text("Textures:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].textures.size());

			for (uint j = 0; j < model_ref->meshes[i].textures.size(); j++)
			{
				ImGui::PushID(j);
				if (ImGui::ImageButtonHover((ImTextureID*)model_ref->meshes[i].textures[j].id, ImVec2(100, 100), ImVec2(-1, 1), ImVec2(0, 0)) 
					&& App->input->dropped)
				{
					App->fbx_loader->SetInfoToLoadTex(i, j, model_ref->meshes[i].textures[j].id);
				}
				ImGui::PopID();
			}
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
}

bool Inspector::CleanUp()
{

	return true;
}
