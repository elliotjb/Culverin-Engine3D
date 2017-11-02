#pragma once
#include "CompMaterial.h"
#include "Color.h"
#include "GAmeObject.h"
#include "parson.h"

CompMaterial::CompMaterial(Comp_Type t, GameObject* parent): Component(t, parent)
{
	color = White;
}

CompMaterial::~CompMaterial()
{
}

void CompMaterial::SetColor(float r, float g, float b, float a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

Color CompMaterial::GetColor() const
{
	return color;
}

void CompMaterial::SetTexture(std::vector<Texture> textures)
{
	texture = textures;
}

void CompMaterial::AddTexture(const Texture tex)
{
	texture.push_back(tex);
}

void CompMaterial::ShowInspectorInfo()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();

		ImGui::ColorEdit3("", (float*)&color);

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
}

void CompMaterial::Save(JSON_Object* object, std::string name) const
{
	json_object_dotset_number_with_std(object, name + "Type", C_MATERIAL);
}

void CompMaterial::Load(const JSON_Object * object, std::string name)
{
}
