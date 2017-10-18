#pragma once
#include "Module.h"
#include "Application.h"
#include "ModuleImporter.h"

struct Texture;

class ImportMesh : public ModuleImporter
{
public:
	ImportMesh();
	~ImportMesh();

	//bool Import(const char* file, const char* path, std::string& output_file);
	bool Load(const char* exported_file, Texture* resource);
};

