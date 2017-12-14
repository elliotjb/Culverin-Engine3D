#include "ImportScript.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ModuleImporter.h"
#include "ModuleGUI.h"
#include "ResourceScript.h"
#include "ModuleResourceManager.h"
#include "JSONSerialization.h"
#include "CSharpScript.h"
#include "Globals.h"

#include <direct.h>
#pragma comment(lib, "mono-2.0-sgen.lib")

ImportScript::ImportScript()
{

}


ImportScript::~ImportScript()
{
}

bool ImportScript::InitScriptingSystem()
{
	char my_path[FILENAME_MAX];
	// Fill my_path char array with the path of the .dll
	_getcwd(my_path, FILENAME_MAX);

	// Set where Mono Directory is placed in mono_path
	mono_path = my_path;
	mono_path += "/Mono";

	// Use the standard configuration ----
	mono_config_parse(NULL);

	// Get the correct dirs --------------
	std::string lib = mono_path;
	lib += "/lib";
	std::string etc = mono_path;
	etc += "/etc";


	// Setup the default directories for mono use here for now the directories of your Mono installation
	mono_set_dirs(lib.c_str(), etc.c_str());
	domain = mono_jit_init_version("Scripting", "v4.0.30319");

	MonoAssembly* culverin_assembly = mono_domain_assembly_open(domain, "./ScriptManager/AssemblyReference/CulverinEditor.dll");
	if (culverin_assembly)
	{
		culverin_mono_image = mono_assembly_get_image(culverin_assembly);
		return true;
	}
	return false;
}

void ImportScript::AddInternalCalls(const char * function_name, const void * link_method)
{
	////GAMEOBJECT FUNCTIONS ---------------
	//mono_add_internal_call("CulverinEditor.GameObject::CreateGameObject",(const void*)CreateGameObject);
	//mono_add_internal_call("CulverinEditor.GameObject::Destroy",(const void*)DestroyGameObject);
	//mono_add_internal_call("CulverinEditor.GameObject::SetActive",(const void*)SetActive);
	//mono_add_internal_call("CulverinEditor.GameObject::IsActive",(const void*)IsActive);
	//mono_add_internal_call("CulverinEditor.GameObject::SetParent",(const void*)SetParent);
	//mono_add_internal_call("CulverinEditor.GameObject::SetName",(const void*)SetName);
	//mono_add_internal_call("CulverinEditor.GameObject::GetName",(const void*)GetName);
	//mono_add_internal_call("CulverinEditor.GameObject::AddComponent",(const void*)AddComponent);
	//mono_add_internal_call("CulverinEditor.GameObject::GetComponent",(const void*)GetComponent);

	////CONSOLE FUNCTIONS ------------------
	//mono_add_internal_call("CulverinEditor.Console.Console::Log",(const void*)ConsoleLog);

	////INPUT FUNCTIONS -------------------
	//mono_add_internal_call("CulverinEditor.Input::KeyDown",(const void*)KeyDown);
	//mono_add_internal_call("CulverinEditor.Input::KeyUp",(const void*)KeyUp);
	//mono_add_internal_call("CulverinEditor.Input::KeyRepeat",(const void*)KeyRepeat);
	//mono_add_internal_call("CulverinEditor.Input::MouseButtonDown",(const void*)MouseButtonDown);
	//mono_add_internal_call("CulverinEditor.Input::MouseButtonUp",(const void*)MouseButtonUp);
	//mono_add_internal_call("CulverinEditor.Input::MouseButtonRepeat",(const void*)MouseButtonRepeat);
}

bool ImportScript::Import(const char* file, uint uuid)
{
	uint uuid_mesh = 0;
	if (uuid == 0) // if direfent create a new resource with the resource deleted
	{
		uuid_mesh = App->random->Int();
	}
	else
	{
		uuid_mesh = uuid;
	}
	ResourceScript* res_script = (ResourceScript*)App->resource_manager->CreateNewResource(Resource::Type::SCRIPT, uuid_mesh);
	if (res_script != nullptr)
	{
		std::string fileassets = App->fs->CopyFileToAssetsS(file); //todo no same name!!!
		std::string path_dll;
		// First Compile The CSharp
		if (CompileScript(fileassets.c_str(), path_dll) != 0)
		{
			LOG("[error] Script: %s, Not Compiled", App->fs->GetOnlyName(fileassets).c_str());
			return false;
		}
		else
		{
			LOG("Script: %s, Compiled without errors", App->fs->GetOnlyName(fileassets).c_str());
			//now 
			CSharpScript* newCSharp = LoadScript_CSharp(path_dll);
			res_script->SetCSharp(newCSharp);
		}


		// Then Create Meta
		std::string Newdirectory = ((Project*)App->gui->winManager[WindowName::PROJECT])->GetDirectory();
		Newdirectory += "\\" + App->fs->FixName_directory(file);
		App->Json_seria->SaveScript(res_script, ((Project*)App->gui->winManager[WindowName::PROJECT])->GetDirectory(), Newdirectory.c_str());

	}
	return true;
}


MonoDomain * ImportScript::GetDomain()
{
	return domain;
}

MonoImage* ImportScript::GetCulverinImage()
{
	return culverin_mono_image;
}

std::string ImportScript::GetMonoPath() const
{
	return mono_path;
}

int ImportScript::CompileScript(const char* file, std::string& libraryScript)
{
	// Get the path of the project ----
	std::string script_path = file;

	// Get mono directory -------------
	std::string command = GetMonoPath();

	// Save dll to Library Directory --------------------
	libraryScript = App->fs->GetFullPath("Library/Scripts/");
	std::string nameFile = App->fs->GetOnlyName(file);
	nameFile += ".dll";
	libraryScript += nameFile;

	// Compile the script -----------------------------
	command += "/monobin/mcs -debug -target:library -out:" + libraryScript + " ";
	std::string CulverinEditorpath = App->fs->GetFullPath("ScriptManager/Assembly Reference/CulverinEditor.dll");
	command += "-r:" + CulverinEditorpath + " ";
	command += "-lib:" + CulverinEditorpath + " ";
	command += script_path;
	//ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false); -> Hide console (good or bad?)
	return system(command.c_str());
	//system("pause");
}

CSharpScript* ImportScript::LoadScript_CSharp(std::string file)
{
	if (file != "")
	{
		MonoAssembly* assembly = mono_domain_assembly_open(GetDomain(), file.c_str());
		if (assembly)
		{
			// First Get the Image
			MonoImage* image = mono_assembly_get_image(assembly);
			if (image)
			{
				CSharpScript* csharp = CreateCSharp(image);
				if (csharp != nullptr)
				{
					csharp->LoadScript();
					return csharp;
				}
				else
				{
					LOG("[error] Can not create MonoClass, file: %s ", file.c_str());
				}
			}
			else
			{
				LOG("[error] Can not Get Image, file: %s ", file.c_str());
			}
		}
		else
		{
			LOG("[error] Can not Open Assembly, file: %s ", file.c_str());
		}
	}
}

CSharpScript* ImportScript::CreateCSharp(MonoImage* image)
{
	if (image)
	{
		std::string classname, name_space;
		MonoClass* entity = GetMonoClassFromImage(image, name_space, classname);
		if (entity)
		{
			CSharpScript* csharp = new CSharpScript();
			csharp->SetDomain(GetDomain());
			csharp->SetImage(image);
			csharp->SetClass(entity);
			csharp->SetClassName(classname);
			csharp->SetNameSpace(name_space);
			return csharp;
		}
	}
	return nullptr;
}


MonoClass* ImportScript::GetMonoClassFromImage(MonoImage* image, std::string& name_space, std::string& classname)
{
	MonoClass* entity = nullptr;
	const MonoTableInfo* table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int rows = mono_table_info_get_rows(table_info);
	/* For each row, get some of its values */
	for (int i = 0; i < rows; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
		const char* classnameCS = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
		const char* name_spaceCS = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		entity = mono_class_from_name(image, name_spaceCS, classnameCS);
		if (entity)
		{
			name_space = name_spaceCS;
			classname = classnameCS;
		}
	}
	return entity;
}