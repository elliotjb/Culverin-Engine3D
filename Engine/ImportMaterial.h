#pragma once
#include "Module.h"
#include "Application.h"
#include "ModuleImporter.h"

struct Texture;

class ImportMaterial
{
public:
	ImportMaterial();
	~ImportMaterial();

	//bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const char* file);
	bool Load(const char* exported_file, Texture* resource);
	bool LoadCheckers(Texture* resource);


};

