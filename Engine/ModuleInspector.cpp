#include "ModuleInspector.h"
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
	if (!BeginDock("INSPECTOR", &App->gui->winManager[INSPECTOR]->active[0].active, ImGuiWindowFlags_HorizontalScrollbar)) //TODO ELLIOT CLOSE Windows example
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

void Inspector::SetInfo(uint m_num, uint v_num, uint f_num, uint t_id, float3 pos, float3 rot, float3 scale)
{
	mesh_num = m_num;
	vertex_num = v_num;
	faces_num = f_num;
	tex_id = t_id;

	object_pos = pos;
	object_rot = rot;
	object_scale = scale;
}

void Inspector::ShowModelInfo() const
{
	ImGui::Text("MODEL INFO");
	if (ImGui::CollapsingHeader("Transformation"))
	{
		ImGui::Text("Position:"); 
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_pos.x, object_pos.y, object_pos.z);
		ImGui::Text("Rotation:"); 
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_rot.x, object_rot.y, object_rot.z);
		ImGui::Text("Size"); 
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "X = %.4f Y = %.4f Z = %.4f", object_scale.x, object_scale.y, object_scale.z);
	}
	if (ImGui::CollapsingHeader("Geometry"))
	{
		ImGui::Text("Total Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", vertex_num);
		ImGui::Text("Total Meshes:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", mesh_num);
		ImGui::Text("Total Faces:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", faces_num);
	}
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Texture:");
		ImGui::Image((ImTextureID*)tex_id, ImVec2(150, 150));
	}
}

bool Inspector::CleanUp()
{

	return true;
}
