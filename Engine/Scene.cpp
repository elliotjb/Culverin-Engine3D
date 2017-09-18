#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "imgui.h"
#include "Gl3W\include\glew.h"
#include "imgui_impl_sdl_gl3.h"

Scene::Scene(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

Scene::~Scene()
{
}

bool Scene::Start()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	return true;
}

update_status Scene::Update(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	ImGui::ShowTestWindow();

	// CLOSE APP WINDOW --------
	ImGui::Begin("CLOSE APP"); 

	static int clicked = 0;
	if (ImGui::Button("CLICK HERE"))
		clicked++;
	if (clicked & 1)
	{
		return UPDATE_STOP;
	}

	static int i1 = 0;
	ImGui::SliderInt("slider int", &i1, 0, 5);
	static float f1 = 0.123f, f2 = 0.0f;
	ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "%.3f");

	ImGui::End();
	// --------------------------

	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}
