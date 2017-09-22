#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "imgui.h"
#include "Gl3W\include\glew.h"
#include "imgui_impl_sdl_gl3.h"

Scene::Scene(bool start_enabled) : Module(start_enabled)
{
}

Scene::~Scene()
{
}

bool Scene::Start()
{
	



	return true;
}

update_status Scene::Update(float dt)
{


	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}
