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
	if (!BeginDock("INSPECTOR", NULL, ImGuiWindowFlags_HorizontalScrollbar)) //TODO ELLIOT CLOSE Windows example
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

void Inspector::SetInfo(const _Model& model)
{
	mesh_num = model.GetTotalMeshes();
	vertex_num = model.GetTotalVertex();
	faces_num = model.GetTotalFaces();
	tex_id = model.GetTexID();

	object_pos = model.GetPosition();
	object_rot = model.GetRotation();
	object_scale = model.GetScale();
}

void Inspector::SetTexInfo(uint texID)
{
	tex_id = texID;
}

void Inspector::ShowModelInfo() const
{
	ImGui::Text("MODEL INFO");
	//ImGuiStyleVar_FramePadding
	//ImGuiInputTextFlags_ReadOnly
	//ImGui::PushStyleVar()
	if (ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 pos = object_pos;//70
		float3 rot = object_rot;//70
		float3 scale = object_scale;//70
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
	}
	if (ImGui::CollapsingHeader("Geometry", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Total Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", vertex_num);
		ImGui::Text("Total Meshes:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", mesh_num);
		ImGui::Text("Total Faces:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", faces_num);
	}
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Texture:");
		ImGui::Image((ImTextureID*)tex_id, ImVec2(150, 150), ImVec2(-1, 1), ImVec2(0, 0));
	}
}

bool Inspector::CleanUp()
{

	return true;
}
