#include "ModuleImporter.h"



ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
	//char ownPth[MAX_PATH];

	//// Will contain exe path
	//HMODULE hModule = GetModuleHandle(NULL);
	//if (hModule != NULL)
	//{
	//	// When passing NULL to GetModuleHandle, it returns handle of exe itself
	//	GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
	//}
}


ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init(JSON_Object* node)
{
	// Will contain exe path
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// When passing NULL to GetModuleHandle, it returns handle of exe itself
		GetModuleFileName(hModule, directoryExe, (sizeof(directoryExe)));
	}

	return true;
}

bool ModuleImporter::Start()
{



	return true;
}

update_status ModuleImporter::PreUpdate(float dt)
{
	perf_timer.Start();

	//
	//if (App->input->dropped)
	//{
	//	dropped_Filetype = CheckFileType(App->input->dropped_filedir);

	//	switch (dropped_Filetype)
	//	{
	//	case F_MODEL:
	//	{
	//		//LOG("IMPORTING MODEL, File Path: %s", App->input->dropped_filedir);
	//		//LoadMesh(App->input->dropped_filedir);

	//		////Fix Camera to model ------------------------
	//		//App->camera->LookAndMoveToObject();
	//		//// ---------------------------------

	//		//SDL_free(App->input->dropped_filedir);

	//		break;
	//	}
	//	case F_TEXTURE:
	//	{
	//		//LOG("IMPORTING TEXTURE, File Path: %s", App->input->dropped_filedir);

	//		////Check if there's a Geometry to apply the dropped texture
	//		//if (App->geometry_manager->geometry != nullptr)
	//		//{
	//		//	/*Delete the previous textures to apply the new one*/
	//		//	((_Model*)(App->geometry_manager->geometry))->ClearTexture(mesh_index, tex_index, tex_id);
	//		//	((_Model*)(App->geometry_manager->geometry))->SetTexture(App->input->dropped_filedir, mesh_index, tex_index, tex_id);

	//		//}
	//		//else
	//		//{
	//		//	LOG("FAIL LOADING TEXTURE: No model to apply it");
	//		//}

	//		//SDL_free(App->input->dropped_filedir);

	//		//mesh_index = -1;
	//		//tex_index = -1;

	//		break;
	//	}
	//	case F_UNKNOWN:
	//	{
	//		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "UNKNOWN file type dropped on window",
	//			App->input->dropped_filedir, App->window->window);
	//		LOG("UNKNOWN FILE TYPE, File Path: %s", App->input->dropped_filedir);

	//		break;
	//	}

	//	default:
	//		break;

	//	}

	//	App->input->dropped = false;
	//}

	Update_t = perf_timer.ReadMs();


	return UPDATE_CONTINUE;
}

update_status ModuleImporter::Update(float dt)
{



	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{



	return UPDATE_CONTINUE;
}

update_status ModuleImporter::UpdateConfig(float dt)
{



	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{



	return true;
}

//FileType ModuleImporter::CheckFileType(char* filedir)
//{
//	if (filedir != nullptr)
//	{
//		std::string file_type;
//		std::string path(filedir);
//
//		size_t extension_pos = path.find_last_of(".");
//
//		file_type = path.substr(extension_pos + 1);
//
//		//Set lowercase the extension to normalize it
//		for (std::string::iterator it = file_type.begin(); it != file_type.end(); it++)
//		{
//			*it = tolower(*it);
//		}
//
//		if (file_type == "png" || file_type == "jpg" || file_type == "dds")
//		{
//			return F_TEXTURE;
//		}
//		else if (file_type == "fbx" || file_type == "obj")
//		{
//			return F_MODEL;
//		}
//		else
//		{
//			return F_UNKNOWN;
//		}
//	}
//	return F_UNKNOWN;
//}