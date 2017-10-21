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
	CompMaterial(Comp_Type t);
	~CompMaterial();

	void SetColor(float r, float g, float b, float a);
	Color GetColor() const;
	
	void ShowInspectorInfo();

private:
	Color color;
	Tex texture;
};