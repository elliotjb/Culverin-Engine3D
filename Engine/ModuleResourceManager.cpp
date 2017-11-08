#include "ModuleResourceManager.h"
#include "Application.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "ResourcePrefab.h"


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

Resource* ModuleResourceManager::CreateNewResource(Resource::Type type)
{
	Resource* ret = nullptr;
	uint uid = App->random->Int();

	switch (type) {
	case Resource::MATERIAL: ret = (Resource*) new ResourceMaterial(uid); break;
	case Resource::MESH: ret = (Resource*) new ResourceMesh(uid); break;
	case Resource::PREFAB: ret = (Resource*) new ResourcePrefab(uid); break;
	}
	if (ret != nullptr)
		resources[uid] = ret;
	return ret;
}
