#pragma once
#include "Module.h"
#include "Application.h"
#include "ModuleLoader.h"


class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	FileType CheckFileType(char* filedir);
	bool Import();

public:
	char directoryExe[MAX_PATH];
	//FileType dropped_Filetype = F_UNKNOWN;
};

