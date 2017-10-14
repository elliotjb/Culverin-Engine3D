#include "ModuleFBXLoader.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGeometries.h"
#include "WindowManager.h"
#include "ModuleInspector.h"
#include "ModuleCamera3D.h"
#include "_Model.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include <string>

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
		dropped_Filetype = CheckFileType(App->input->dropped_filedir);

		switch (dropped_Filetype)
		{
		case F_MODEL:
		{
			LOG("IMPORTING MODEL, File Path: %s", App->input->dropped_filedir);
			LoadMesh(App->input->dropped_filedir);

			//Fix Camera to model ------------------------
			App->camera->LookAndMoveToObject();
			// ---------------------------------

			SDL_free(App->input->dropped_filedir);

			break;
		}
		case F_TEXTURE:
		{
			LOG("IMPORTING TEXTURE, File Path: %s", App->input->dropped_filedir);

			//Check if there's a Geometry to apply the dropped texture
			if (App->geometry_manager->geometry != nullptr)
			{
				((_Model*)(App->geometry_manager->geometry))->SetTexture(App->input->dropped_filedir);
			}

			//Fix Camera to model ------------------------
			App->camera->LookAndMoveToObject();
			// ---------------------------------

			SDL_free(App->input->dropped_filedir);

			break;
		}
		case F_UNKNOWN:
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "UNKNOWN file type dropped on window", 
				App->input->dropped_filedir, App->window->window);
			LOG("UNKNOWN FILE TYPE, File Path: %s", App->input->dropped_filedir);
			
			break;
		}

		default:
			break;

		}

		App->input->dropped = false;
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

FileType ModuleFBXLoader::CheckFileType(char * filedir)
{
	if (filedir != nullptr)
	{
		std::string file_type;
		std::string path(filedir);

		size_t extension_pos = path.find_last_of(".");

		file_type = path.substr(extension_pos + 1);

		//Set lowercase the extension to normalize it
		for (std::string::iterator it = file_type.begin(); it != file_type.end(); it++)
		{
			*it = tolower(*it);
		}

		if (file_type == "png" || file_type == "jpg" || file_type == "dds")
		{
			return F_TEXTURE;
		}
		else if (file_type == "fbx" || file_type == "obj")
		{
			return F_MODEL;
		}
		else
		{
			return F_UNKNOWN;
		}
	}
}

BaseGeometry* ModuleFBXLoader::LoadMesh(const char* filename)
{
	_Model* new_model = new _Model(filename, P_MODEL, App->renderer3D->wireframe);
	//new_model->id = App->geometry_manager->count++;
	//App->geometry_manager->objects.push_back(new_model);

	if (App->geometry_manager->geometry != nullptr)
	{
		App->geometry_manager->geometry->Clear();
		RELEASE(App->geometry_manager->geometry);
	}

	((Inspector*)App->gui->winManager[INSPECTOR])->model_loaded = true;
	((Inspector*)App->gui->winManager[INSPECTOR])->SetInfo(new_model);
	
	App->geometry_manager->geometry = new_model;

	return m;
}

