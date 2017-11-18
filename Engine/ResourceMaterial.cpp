#include "ResourceMaterial.h"
#include "Application.h"


ResourceMaterial::ResourceMaterial(uint uuid) : Resource(uuid, Resource::Type::MATERIAL, Resource::State::UNLOADED)
{
	NumGameObjectsUseMe = 0;
}


ResourceMaterial::~ResourceMaterial()
{
}

void ResourceMaterial::InitInfo(const char* nameResource)
{
	name = App->GetCharfromConstChar(nameResource);
}

void ResourceMaterial::Init(Texture textureloaded)
{
	texture.id = textureloaded.id;
	texture.name = textureloaded.name;
}

void ResourceMaterial::DeleteToMemory()
{
	state = Resource::State::UNLOADED;
	glDeleteTextures(1, &texture.id);
}

bool ResourceMaterial::LoadToMemory()
{
	state = Resource::State::LOADED;
	return true;
}

uint ResourceMaterial::GetTextureID()
{
	if (texture.id > 0)
	{
		return texture.id;
	}
}


Resource::State ResourceMaterial::IsLoadedToMemory()
{
	return state;
}






