#pragma once
#include "Module.h"

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool start_enabled = true);
	virtual ~ModuleResourceManager();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:

};