#include "ResourceMaterial.h"
#include "Application.h"


ResourceMaterial::ResourceMaterial(uint uuid) : Resource(uuid, Resource::Type::MATERIAL, Resource::State::UNLOADED)
{
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


bool ResourceMaterial::IsLoadedToMemory()
{
	return state;
}






