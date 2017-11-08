#pragma once
#include "Module.h"
#include "Resource_.h"
#include <map>

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool start_enabled = true);
	virtual ~ModuleResourceManager();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Resource* CreateNewResource(Resource::Type type);

private:
	uint last_uid = 1;
	std::map<uint, Resource*> resources;
};