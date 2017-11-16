#include "ResourceMaterial.h"
#include "Application.h"


ResourceMaterial::ResourceMaterial(uint uid) : Resource(uid, Resource::Type::MATERIAL)
{
}


ResourceMaterial::~ResourceMaterial()
{
}

void ResourceMaterial::InitInfo(uint uuid, const char* nameResource)
{
	name = App->GetCharfromConstChar(nameResource);
	uuid_mesh = uuid;
}

void ResourceMaterial::Init(Texture textureloaded)
{
	texture.id = textureloaded.id;
	texture.name = textureloaded.name;
}

bool ResourceMaterial::LoadToMemory()
{
	isLoaded = true;
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
	return isLoaded;
}






