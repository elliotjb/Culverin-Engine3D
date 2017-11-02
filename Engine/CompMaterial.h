#pragma once
#include "Component.h"
#include "Color.h"
#include <string>

struct Tex
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
	
	void ShowInspectorInfo();

	void Save(JSON_Object* object, std::string name) const;
	void Load(const JSON_Object* object, std::string name);

private:
	Color color;
	Tex texture;
};