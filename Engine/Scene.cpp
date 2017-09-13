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
	return false;
}

update_status Scene::Update(float dt)
{
	return update_status();
}

bool Scene::CleanUp()
{
	return false;
}
