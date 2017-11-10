#pragma once
#include "Module.h"
#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")

class ImportMesh;
class ImportMaterial;
class CompTransform;

enum FileTypeImport
{
	F_UNKNOWN_i = -1,
	F_MODEL_i,
	F_TEXTURE_i
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	GameObject* ProcessNode(aiNode* node, const aiScene* scene, GameObject* obj);
	void ProcessTransform(aiNode* node, CompTransform* trans);
	void ProcessTransform(CompTransform* trans);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	FileTypeImport CheckFileType(char* filedir);
	//bool Import();

public:
	char directoryExe[MAX_PATH];
	FileTypeImport dropped_File_type = F_UNKNOWN_i;

	ImportMesh* iMesh = nullptr;
	ImportMaterial* iMaterial = nullptr;
};

