#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "imgui.h"
#include "Primitive.h"
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
	Plane_p p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}
