#include "ModuleFBXLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Globals.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled)
{
}

ModuleFBXLoader::~ModuleFBXLoader()
{
}

bool ModuleFBXLoader::Start()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);



	return true;
}

update_status ModuleFBXLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleFBXLoader::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleFBXLoader::LoadMesh(const char* filename)
{
	bool ret = false;
	aiMesh* new_mesh = nullptr;
	const aiScene* scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		//iterate all aiMesh structs
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			m = new BaseObject(P_MESH);
			new_mesh = scene->mMeshes[i];

			m->mesh.num_vertices = new_mesh->mNumVertices;
			m->mesh.vertices = new float[m->mesh.num_vertices * 3];
			memcpy(m->mesh.vertices, new_mesh, sizeof(float) * m->mesh.num_vertices * 3);
		
			LOG("New mesh with %d vertices", m->mesh.num_vertices);
		
			if (new_mesh->HasFaces())
			{
				m->mesh.num_indices = new_mesh->mNumFaces * 3;
				m->mesh.indices = new uint[m->mesh.num_indices * 3]; //Each face is a triangle
				
				for (uint j = 0; j < new_mesh->mNumFaces; ++j)
				{
					if (new_mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->mesh.indices[j * 3], new_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}

				//Put the new mesh in the vector
				meshes_vec.push_back(m);
			}
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", filename);
	}

	return ret;
}

