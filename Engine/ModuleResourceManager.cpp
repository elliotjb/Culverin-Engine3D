#include "ModuleResourceManager.h"

ModuleResourceManager::ModuleResourceManager(bool start_enabled): Module(start_enabled)
{
	name = "Resources Manager";
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Start()
{
	return true;
}

update_status ModuleResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}
