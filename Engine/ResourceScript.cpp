#include "ResourceScript.h"
#include "CSharpScript.h"
#include "Application.h"
#include "ModuleFS.h"
#include "Script_editor.h"

ResourceScript::ResourceScript(uint uid) : Resource(uid, Resource::Type::SCRIPT, Resource::State::UNLOADED)
{
	NumGameObjectsUseMe = 0;
	editor = new Script_editor(this);
	LOG("Recource Material Created!");
}

ResourceScript::~ResourceScript()
{
	path_dll.clear();
	path_assets.clear();
	RELEASE(editor);
}

void ResourceScript::InitInfo(std::string path_dll_, std::string path_assets_)
{
	name = App->GetCharfromConstChar(App->fs->GetOnlyName(path_assets_).c_str());
	path_dll = path_dll_;
	path_assets = path_assets_;
}

void ResourceScript::CreateNewScript(std::string nameScript)
{
	editor->Start(nameScript, true);
	editor->SaveScript();
}

void ResourceScript::SetScriptEditor(std::string nameScript)
{
	editor->Start(nameScript, false);
}

bool ResourceScript::ReImportScript(std::string path_dll_)
{
	return csharp->ReImport(path_dll_);
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

std::string ResourceScript::GetPathdll() const
{
	return path_dll;
}

void ResourceScript::ShowEditor(bool& active)
{
	editor->Show(active);
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
