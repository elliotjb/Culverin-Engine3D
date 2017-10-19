#pragma once
#include "Globals.h"
#include "Module.h"

class ModuleFS : public Module
{
public:
	ModuleFS(bool start_enabled = true);
	~ModuleFS();

	bool Init(JSON_Object* node);
	bool Start();

	//update_status PreUpdate(float dt);
	//update_status Update(float dt);
	//update_status PostUpdate(float dt);
	//update_status UpdateConfig(float dt);

	uint LoadFile(const char* file, char** buffer);
};

