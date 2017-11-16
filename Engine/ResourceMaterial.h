#pragma once
#include "Resource_.h"
#include <string>

struct Texture
{
	uint id;
	//Const char* ?
	std::string nameExt;
	//std::string path;
	std::string name;

};

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(uint uid);
	virtual ~ResourceMaterial();

	void InitInfo(uint uuid, const char* name);
	void Init(Texture texture);

	bool LoadToMemory();
	uint GetTextureID();
	bool IsLoadedToMemory();

private:
	bool isLoaded = false;
	Texture texture;
};

