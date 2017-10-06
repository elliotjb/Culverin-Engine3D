#include "ModuleFBXLoader.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled): Module(start_enabled)
{
	name = "FBX Loader";
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
	//TODO -> place it in postUpdate method
	if (App->input->dropped)
	{
		LoadMesh(App->input->dropped_filedir);
		App->input->dropped = false;
	}

	for (uint i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::postUpdate(float dt)
{
	if (App->input->dropped)
	{
		LoadMesh(App->input->dropped_filedir);
		App->input->dropped = false;
	}

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
			memcpy(m->mesh.vertices, new_mesh->mVertices, sizeof(float) * m->mesh.num_vertices * 3);
		
			LOG("New mesh with %d vertices", m->mesh.num_vertices);
		
			if (new_mesh->HasFaces())
			{
				m->mesh.num_indices = new_mesh->mNumFaces * 3;
				m->mesh.indices = new uint[m->mesh.num_indices]; //Each face is a triangle
				
				for (uint j = 0; j < new_mesh->mNumFaces; j++)
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

				//Save space in VRAM and add the new mesh in the vector
				m->Init();
				meshes.push_back(m);
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

