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
	if (!BeginDock("Inspector", &App->gui->winManager[INSPECTOR]->active[0].active, ImGuiWindowFlags_HorizontalScrollbar)) //TODO ELLIOT CLOSE Windows example
	{
		EndDock();
		return;
	}
	ImGui::Text("HELLO WORLD");

	static bool ref_color = false;
	static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);

	ImGui::Text("Color widget HSV with Alpha:");
	static ImVec4 color = ImColor(114, 144, 154, 200);
	static bool hdr = false;
	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool options_menu = true;
	int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

	ImGui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_HSV | misc_flags);



	EndDock();
}

bool Inspector::CleanUp()
{

	return true;
}
