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
	bool Import(const char* file, const char* name_file);
	bool Load(const char* exported_file, CompMaterial* materialComp);
	bool LoadCheckers(Texture* resource);


};

