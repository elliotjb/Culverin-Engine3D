#include "ModuleFBXLoader.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGeometries.h"
#include "_Model.h"
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

	//for (uint i = 0; i < meshes.size(); i++)
	//{
	//	meshes[i]->Draw();
	//}

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

BaseGeometry* ModuleFBXLoader::LoadMesh(const char* filename)
{
	_Model* new_model = new _Model(filename, P_MODEL, App->renderer3D->wireframe);
	new_model->id = App->geometry_manager->count++;
	App->geometry_manager->objects.push_back(new_model);

	return m;
}

