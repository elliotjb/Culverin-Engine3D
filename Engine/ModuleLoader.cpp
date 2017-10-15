#include "ModuleLoader.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGeometries.h"
#include "WindowManager.h"
#include "ModuleInspector.h"
#include "ModuleHierarchy.h"
#include "ModuleCamera3D.h"
#include "_Model.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include <string>

#pragma comment(lib, "Assimp/libx86/assimp.lib")

ModuleLoader::ModuleLoader(bool start_enabled): Module(start_enabled)
{
	Start_enabled = true;
	Update_enabled = true;

	name = "FBX Loader";
}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Start()
{

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

update_status ModuleLoader::Update(float dt)
{
	perf_timer.Start();

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
				/*Delete the previous textures to apply the new one*/
				((_Model*)(App->geometry_manager->geometry))->ClearTexture(mesh_index, tex_index, tex_id);
				((_Model*)(App->geometry_manager->geometry))->SetTexture(App->input->dropped_filedir, mesh_index, tex_index, tex_id);

			}
			else
			{
				LOG("FAIL LOADING TEXTURE: No model to apply it");
			}

			//Fix Camera to model ------------------------
			App->camera->LookAndMoveToObject();
			// ---------------------------------

			SDL_free(App->input->dropped_filedir);

			mesh_index = -1;
			tex_index = -1;

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

	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

//update_status ModuleFBXLoader::postUpdate(float dt)
//{
//	if (App->input->dropped)
//	{
//		LoadMesh(App->input->dropped_filedir);
//		App->input->dropped = false;
//	}
//
//	return UPDATE_CONTINUE;
//}

bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

FileType ModuleLoader::CheckFileType(char * filedir)
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

BaseGeometry* ModuleLoader::LoadMesh(const char* filename)
{
	if (App->geometry_manager->geometry != nullptr)
	{
		App->geometry_manager->geometry->Clear();
		RELEASE(App->geometry_manager->geometry);
	}

	_Model* new_model = new _Model(filename, P_MODEL, App->renderer3D->wireframe);

	if (new_model->loaded_success)
	{
		((Inspector*)App->gui->winManager[INSPECTOR])->model_loaded = true;
		((Inspector*)App->gui->winManager[INSPECTOR])->SetInfo(new_model);
		((Hierarchy*)App->gui->winManager[HIERARCHY])->SetName(new_model);

		App->geometry_manager->geometry = new_model;
	}
	else
	{
		new_model->Clear();
		RELEASE(new_model);
	}

	return m;
}

void ModuleLoader::SetInfoToLoadTex(int m, int t, int t_id)
{
	mesh_index = m;
	tex_index = t;
	tex_id = t_id;
}

