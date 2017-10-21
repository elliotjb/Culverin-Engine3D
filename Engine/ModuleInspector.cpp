#include "ModuleInspector.h"
#include "_Model.h"
#include "Application.h"
#include "GameObject.h"

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

	if (selected_object != nullptr)
	{ 
		selected_object->ShowInspectorInfo();
	}

	EndDock();
}

void Inspector::LinkObject(GameObject * obj)
{
	if (obj != nullptr)
	{
		selected_object = obj;
	}
}

//void Inspector::SetInfo(_Model* model)
//{
//	if (model != nullptr)
//	{
//		model_ref = model;
//
//		std::string temp = model_ref->base_info.name.c_str();
//		model_name = new char[temp.size() + 1];
//		std::copy(temp.begin(), temp.end(), model_name);
//		model_name[temp.size()] = '\0';
//	}
//}
//
//void Inspector::SetTexInfo(_Model* model)
//{
//	model_ref = model;
//}

//void Inspector::ShowModelInfo() 
//{
	//ImGui::Text("MODEL INFO");
	//ImGuiStyleVar_FramePadding
	//ImGuiInputTextFlags_ReadOnly
	//ImGui::PushStyleVar()
	//ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth(), 40));
	//ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));
	//if (ImGui::BeginChild(ImGui::GetID("Inspector"), ImVec2(ImGui::GetWindowWidth(), 40)))
	//{
	//	static GLuint icon_GameObject = App->textures->LoadTexture("Images/UI/icon_GameObject.png");
	//	ImGui::Spacing();
	//	//ImGui
	//	ImGui::Text(""); ImGui::SameLine(5);
	//	ImGui::Image((ImTextureID*)icon_GameObject, ImVec2(23, 23), ImVec2(-1, 1), ImVec2(0, 0)); ImGui::SameLine();
	//	static bool active = true;
	//	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
	//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
	//	if (ImGui::Checkbox("##1", &active))
	//	{
	//		((BaseGeometry*)model_ref)->Enable(active);
	//	}
	//	ImGui::SameLine();
	//	ImGui::PopStyleVar();
	//	ImGui::InputText("##nameModel", model_name, 256, ImGuiInputTextFlags_ReadOnly);
	//	ImGui::SameLine(); App->ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");
	//	ImGui::PopStyleVar();
	//}

	//ImGui::EndChild();
	//ImGui::PopStyleColor();

	//ImGui::Separator();

	//static bool bounding_box = false;
	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
	//ImGui::Text(""); ImGui::SameLine(8);
	//if (ImGui::Checkbox("##2", &bounding_box))
	//{
	//	model_ref->EnableBoundingBox(bounding_box);
	//}
	//ImGui::SameLine();
	//ImGui::PopStyleVar();
	//ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "Bounding Box");
	//ImGui::PopStyleVar();

	//ImGui::Separator();

	//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	//if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	ImGui::PopStyleColor();
	//	float3 pos = model_ref->base_info.position; //70
	//	float3 rot = model_ref->base_info.rotation; //70
	//	float3 scale = model_ref->base_info.scale; //70
	//	int op = ImGui::GetWindowWidth() / 4;
	//	ImGui::PushItemWidth(op - 40);
	//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 1));
	//	ImGui::Text("Position"); ImGui::SameLine(op + 40);
	//	ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_1", &pos.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	//	ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_1", &pos.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	//	ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_1", &pos.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
	//	ImGui::Text("Rotation"); ImGui::SameLine(op + 40);
	//	ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_2", &rot.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	//	ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_2", &rot.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	//	ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_2", &rot.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
	//	ImGui::Text("Scale"); ImGui::SameLine(op + 40);
	//	ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_3", &scale.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	//	ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_3", &scale.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	//	ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_3", &scale.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
	//	ImGui::PopStyleVar();
	//	ImGui::PopItemWidth();
	//	ImGui::TreePop();
	//}
	//else
	//{
	//	ImGui::PopStyleColor();
	//}
	//ImGui::Separator();
//
//	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
//	if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		ImGui::PopStyleColor();
//		
//		if (ImGui::ColorEdit3("", (float*)&color_picker))
//		{
//			model_ref->color.Set(color_picker.x, color_picker.y, color_picker.z, color_picker.w);
//			model_ref->SetColor(model_ref->color);
//		}
//	
//		ImGui::TreePop();
//	}
//	else
//	{
//		ImGui::PopStyleColor();
//	}
//	ImGui::Separator();
//
//	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
//	if (ImGui::TreeNodeEx("Total Geometry", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		ImGui::PopStyleColor();
//		ImGui::Text("Total Vertices:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_vertex);
//		ImGui::Text("Total Meshes:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_meshes);
//		ImGui::Text("Total Faces:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->base_info.total_faces);
//		ImGui::TreePop();
//	}
//	else
//	{
//		ImGui::PopStyleColor();
//	}
//	ImGui::Separator();
//
//	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
//	if (ImGui::TreeNodeEx("Meshes", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		ImGui::PopStyleColor();
//		for (uint i = 0; i < model_ref->meshes.size(); i++)
//		{
//			ImGui::PushID(i);
//			ImGui::Text("Mesh %i: ", i); ImGui::SameLine();
//			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%s", model_ref->meshes[i].name.c_str());
//			
//			ImGui::Text("Vertices:"); ImGui::SameLine();
//			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].vertices.size());
//			ImGui::Text("Faces:"); ImGui::SameLine();
//			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].numFaces);
//
//			int op = ImGui::GetWindowWidth() / 4;
//			ImGui::PushItemWidth(op - 40);
//			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 1));
//			ImGui::Text("Position"); ImGui::SameLine(op + 40);
//			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_1", &model_ref->meshes[i].pos.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
//			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_1", &model_ref->meshes[i].pos.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
//			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_1", &model_ref->meshes[i].pos.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
//			ImGui::Text("Rotation"); ImGui::SameLine(op + 40);
//			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_2", &model_ref->meshes[i].rot.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
//			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_2", &model_ref->meshes[i].rot.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
//			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_2", &model_ref->meshes[i].rot.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
//			ImGui::Text("Scale"); ImGui::SameLine(op + 40);
//			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_3", &model_ref->meshes[i].scale.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
//			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_3", &model_ref->meshes[i].scale.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
//			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_3", &model_ref->meshes[i].scale.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
//			ImGui::PopStyleVar();
//			ImGui::PopItemWidth();
//			
//			ImGui::Text("Textures:"); ImGui::SameLine();
//			ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", model_ref->meshes[i].textures.size());
//
//			for (uint j = 0; j < model_ref->meshes[i].textures.size(); j++)
//			{
//				ImGui::PushID(j);
//				ImGui::Text("Type:"); ImGui::SameLine();
//				ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%s", model_ref->meshes[i].textures[j].type.c_str());
//				ImGui::ImageButton((ImTextureID*)model_ref->meshes[i].textures[j].id, ImVec2(100, 100), ImVec2(-1, 1), ImVec2(0, 0));
//				if (App->input->dropped)
//				{
//					if (ImGui::IsMouseInside(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y), ImVec2(100, 100)))
//					{
//						//App->loader->SetInfoToLoadTex(i, j, model_ref->meshes[i].textures[j].id);
//					}
//				}
//				ImGui::PopID();
//			}
//			ImGui::Separator();
//			ImGui::PopID();
//		}
//		ImGui::TreePop();
//	}
//	else
//	{
//		ImGui::PopStyleColor();
//	}
//	ImGui::Spacing();
//	ImGui::Spacing();
//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 6));
//	ImGui::ButtonEx("Add Component");
//	ImGui::PopStyleVar();
//}

bool Inspector::CleanUp()
{

	return true;
}
