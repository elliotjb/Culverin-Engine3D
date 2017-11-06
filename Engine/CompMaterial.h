#pragma once
#include "Component.h"
#include "Color.h"
#include <string>
#include <vector>

struct Texture
{
	uint id;
	std::string type;
	std::string path;

};

class CompMaterial : public Component
{
public:
	CompMaterial(Comp_Type t, GameObject* parent);
	~CompMaterial();

	void SetColor(float r, float g, float b, float a);
	Color GetColor() const;
	
	void SetTexture(std::vector<Texture> textures);
	void AddTexture(const Texture texture);
	uint GetTextureID();

	void SetUUIDMesh(uint uuid);

	void ShowInspectorInfo();

	void Save(JSON_Object* object, std::string name) const;
	void Load(const JSON_Object* object, std::string name);

private:
	Color color;
	//Texture texture;
	std::vector<Texture> texture;
	uint uuid_material = 0;
};