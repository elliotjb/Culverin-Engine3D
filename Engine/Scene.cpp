#include "Globals.h"
#include "Application.h"
#include "Scene.h"

Scene::Scene(Application * app, bool start_enabled) : Module(app, start_enabled)
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
