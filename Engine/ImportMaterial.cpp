#include "ImportMaterial.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

ImportMaterial::ImportMaterial()
{
}


ImportMaterial::~ImportMaterial()
{
}

//update_status MaterialImporter::Update(float dt)
//{
//
//
//
//	return UPDATE_CONTINUE;
//}

bool ImportMaterial::Import(const char* file, const char* path, std::string& output_file)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG("Error to Import, %s", importer.GetErrorString());
		return false;
	}

	return false;
}

bool ImportMaterial::Import(const void * buffer, uint size, std::string & output_file)
{
	return false;
}

bool ImportMaterial::Load(const char* exported_file, Texture* resource)
{
	return false;
}

bool ImportMaterial::LoadCheckers(Texture* resource)
{
	return false;
}
