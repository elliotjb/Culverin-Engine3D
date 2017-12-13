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

#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/jit/jit.h>


ImportScript::ImportScript()
{
}


ImportScript::~ImportScript()
{
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

int ImportScript::CompileScript(const char* file, std::string& libraryScript)
{
	// Get the path of the project ----
	std::string script_path = file;

	// Get mono directory -------------
	std::string command = App->importer->GetMonoPath();

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
		MonoAssembly* assembly = mono_domain_assembly_open(App->importer->GetDomain(), file.c_str());
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
			csharp->SetDomain(App->importer->GetDomain());
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