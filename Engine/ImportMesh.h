#pragma once
#include "Module.h"
#include "Application.h"
#include "ModuleImporter.h"

struct Texture;

class ImportMesh
{
public:
	ImportMesh();
	~ImportMesh();

	//bool Import(const char* file, std::string& output_file);
	bool Load(const char* exported_file, Texture* resource);

	bool Import(const aiScene * scene, const aiMesh* mesh, GameObject* obj, const char* name);
	void Import(uint num_vertices, uint num_indices, uint num_normals, std::vector<uint> indices, std::vector<float3> vertices);
	//void Import(uint num_vert, uint num_ind, uint num_normals, std::vector<uint> indices, std::vector<Vertex> vertices);

	bool LoadResource(const char * file, ResourceMesh* resourceMesh);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);
	bool Load(const char* name, CompMesh* mesh);
	//Call always before ProseceNode();
	void PrepareToImport();

private:

	std::vector<Texture> materialImpoted; //Const char* ?
};

