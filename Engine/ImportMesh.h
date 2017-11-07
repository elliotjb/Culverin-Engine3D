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

	bool Import(const aiScene * scene, aiMesh* mesh, GameObject* obj, const char * name);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);

	bool Load(const char* name, CompMesh* mesh);
	//Call always before ProseceNode();
	void PrepareToImport();

private:

	std::vector<Texture> materialImpoted; //Const char* ?
};

