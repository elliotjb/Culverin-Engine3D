#include "ImportScript.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ModuleGUI.h"
#include "ResourceScript.h"
#include "ModuleResourceManager.h"
#include "JSONSerialization.h"


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
	std::string Newdirectory = ((Project*)App->gui->winManager[WindowName::PROJECT])->GetDirectory();
	Newdirectory += "\\" + App->fs->FixName_directory(file);
	App->Json_seria->SaveScript(res_script, ((Project*)App->gui->winManager[WindowName::PROJECT])->GetDirectory(), Newdirectory.c_str());
	
	return true;
}
