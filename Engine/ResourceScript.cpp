#include "ResourceScript.h"
#include "CSharpScript.h"
#include "Application.h"
#include "ModuleFS.h"

ResourceScript::ResourceScript(uint uid) : Resource(uid, Resource::Type::SCRIPT, Resource::State::UNLOADED)
{
	NumGameObjectsUseMe = 0;
	LOG("Recource Material Created!");
}

ResourceScript::~ResourceScript()
{
	path_dll.clear();
	path_assets.clear();
}

void ResourceScript::InitInfo(std::string path_dll_, std::string path_assets_)
{
	name = App->GetCharfromConstChar(App->fs->GetOnlyName(path_assets_).c_str());
	path_dll = path_dll_;
	path_assets = path_assets_;
}

void ResourceScript::SetCSharp(CSharpScript* csharp_)
{
	csharp = csharp_;
}

bool ResourceScript::Update(float dt)
{
	if (csharp != nullptr)
	{
		csharp->DoMainFunction(FunctionBase::CS_Update);
	}
	return false;
}

std::string ResourceScript::GetPathAssets() const
{
	return path_assets;
}

void ResourceScript::DeleteToMemory()
{
	//state = Resource::State::UNLOADED;
	//glDeleteTextures(1, &texture.id);
	LOG("UnLoaded Recource Script");
}

bool ResourceScript::LoadToMemory()
{
	state = Resource::State::LOADED;
	LOG("Loaded Recource Script");
	return true;
}

Resource::State ResourceScript::IsCompiled()
{
	return state;
}
