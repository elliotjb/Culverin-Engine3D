#include "ModuleImporter.h"
#include "Application.h"
#include "ImportMesh.h"
#include "ImportMaterial.h"
#include "CompTransform.h"


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
	name = "Importer";
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
	iMesh = new ImportMesh();
	iMaterial = new ImportMaterial();

	return true;
}

bool ModuleImporter::Start()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

update_status ModuleImporter::PreUpdate(float dt)
{
	perf_timer.Start();

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		//ImportMesh* imp = new ImportMesh();
		//imp->Load("Baker_house.rin");
	}

	if (App->input->dropped)
	{
		dropped_File_type = CheckFileType(App->input->dropped_filedir);


		//App->fs->CopyFileToAssets(App->input->dropped_filedir, ((Project*)App->gui->winManager[PROJECT])->GetFolderSee().c_str());
		//((Project*)App->gui->winManager[PROJECT])->AddFile(((Project*)App->gui->winManager[PROJECT])->GetFilesSee(), App->input->dropped_filedir);

		switch (dropped_File_type)
		{
		case F_MODEL_i:
		{
			LOG("IMPORTING MODEL, File Path: %s", App->input->dropped_filedir);
			std::string file;

			const aiScene* scene = aiImportFile(App->input->dropped_filedir, aiProcessPreset_TargetRealtime_MaxQuality);
			GameObject* obj = new GameObject();
			obj->SetName(App->GetCharfromConstChar(App->fs->FixName_directory(App->input->dropped_filedir).c_str()));
			CompTransform* trans = (CompTransform*)obj->AddComponent(C_TRANSFORM);
			trans->SetZero();
			ProcessNode(scene->mRootNode, scene, obj);
			aiReleaseImport(scene);
			App->scene->gameobjects.push_back(obj);
			//Now Save Serialitzate OBJ -> Prefab

			break;
		}
		case F_TEXTURE_i:
		{
			LOG("IMPORTING TEXTURE, File Path: %s", App->input->dropped_filedir);
			iMaterial->Import(App->input->dropped_filedir);
		
			break;
		}
		case F_UNKNOWN_i:
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

void ModuleImporter::ProcessNode(aiNode* node, const aiScene* scene, GameObject* obj)
{
	// Process all the Node's MESHES
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		GameObject* objChild = new GameObject();
		objChild->SetName(App->GetCharfromConstChar(node->mName.C_Str()));
		CompTransform* trans = (CompTransform*)objChild->AddComponent(C_TRANSFORM);
		trans->SetTransformation(node->mTransformation);
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		iMesh->Import(scene, mesh, objChild, node->mName.C_Str());
		obj->AddChildGameObject_Copy(objChild);
	}

	// Process children
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, obj);
	}
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
	aiDetachAllLogStreams();
	return true;
}

FileTypeImport ModuleImporter::CheckFileType(char * filedir)
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
			return F_TEXTURE_i;
		}
		else if (file_type == "fbx" || file_type == "obj")
		{
			return F_MODEL_i;
		}
		else
		{
			return F_UNKNOWN_i;
		}
	}
}
