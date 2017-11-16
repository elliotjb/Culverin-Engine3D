#pragma once
#include "Component.h"
#include "Color.h"
#include <string>

class ResourceMaterial;

class CompMaterial : public Component
{
public:
	CompMaterial(Comp_Type t, GameObject* parent);
	~CompMaterial();

	void SetColor(float r, float g, float b, float a);
	Color GetColor() const;
	
	//void SetTexture(std::vector<Texture> textures);
	//void AddTexture(const Texture texture);
	uint GetTextureID();

	void SetUUIDMesh(uint uuid);

	void ShowOptions();
	void ShowInspectorInfo();

	void Save(JSON_Object* object, std::string name) const;
	void Load(const JSON_Object* object, std::string name);

public:
	ResourceMaterial* resourceMaterial = nullptr;
private:
	Color color;
	bool SelectMesh = false;
	//Texture texture;
	uint uuid_material = 0;
};