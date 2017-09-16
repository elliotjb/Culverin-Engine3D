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

	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}
