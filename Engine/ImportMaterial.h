#pragma once
#include "Module.h"
#include "Application.h"
#include "ModuleImporter.h"

struct Texture;

class ImportMaterial : public ModuleImporter
{
public:
	ImportMaterial();
	~ImportMaterial();

	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file, Texture* resource);
	bool LoadCheckers(Texture* resource);


};

