#include "ResourceScript.h"



ResourceScript::ResourceScript(uint uid) : Resource(uuid, Resource::Type::MATERIAL, Resource::State::UNLOADED)
{
	NumGameObjectsUseMe = 0;
	LOG("Recource Material Created!");
}

ResourceScript::~ResourceScript()
{
}

void ResourceScript::DeleteToMemory()
{
	state = Resource::State::UNLOADED;
	//glDeleteTextures(1, &texture.id);
	LOG("UnLoaded Recource Script");
}

bool ResourceScript::LoadToMemory()
{
	state = Resource::State::LOADED;
	LOG("Loaded Recource Script");
	return true;
}

Resource::State ResourceScript::IsLoadedToMemory()
{
	return state;
}
