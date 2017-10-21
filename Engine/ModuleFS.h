#pragma once
#include "Globals.h"
#include "Module.h"
#include <filesystem>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>

class ModuleFS : public Module
{
public:
	ModuleFS(bool start_enabled = true);
	~ModuleFS();

	bool Init(JSON_Object* node);
	bool Start();

	update_status PreUpdate(float dt);
	std::vector<std::string> Get_filenames(std::experimental::filesystem::path path);
	//update_status Update(float dt);
	//update_status PostUpdate(float dt);
	//update_status UpdateConfig(float dt);

	uint LoadFile(const char* file, char** buffer);
	bool SaveFile(const char* data, std::string name, uint size);

public:
	std::vector<std::string> files;
	char ownPth[MAX_PATH];
};

