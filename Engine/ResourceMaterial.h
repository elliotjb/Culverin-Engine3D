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

	void InitInfo(const char* name);
	void Init(Texture texture);

	bool LoadToMemory();
	uint GetTextureID();
	bool IsLoadedToMemory();

private:
	Texture texture;
};

