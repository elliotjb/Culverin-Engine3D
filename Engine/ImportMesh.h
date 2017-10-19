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

	bool Import(aiScene * scene, std::string & output_file);

	//bool Import(const char* file, std::string& output_file);
	bool Load(const char* exported_file, Texture* resource);

	bool Import(aiMesh * mesh, const char * name);

	bool Load(const char * name);

	//bool Import(aiMesh * mesh);

private:
	uint num_vertices = 0;
	uint num_indices = 0;
	uint num_normals = 0;
	uint num_tex_coords = 0;
	float3* vertices = nullptr;
	uint* indices = nullptr;
	float3* vert_normals = nullptr;
	float2* tex_coords = nullptr;

};

