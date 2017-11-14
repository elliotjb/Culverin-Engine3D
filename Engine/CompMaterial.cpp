#pragma once
#include "CompMaterial.h"
#include "Color.h"
#include "GAmeObject.h"
#include "parson.h"
#include "Application.h"
#include "ImportMaterial.h"

CompMaterial::CompMaterial(Comp_Type t, GameObject* parent): Component(t, parent)
{
	uid = App->random->Int();
	color = White;
}

CompMaterial::~CompMaterial()
{
	for (int i = 0; i < texture.size(); i++)
	{
		texture[i].name.clear();
		texture[i].path.clear();
		texture[i].type.clear();
	}
	texture.clear();
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

uint CompMaterial::GetTextureID()
{
	if(texture.size() > 0)
		return texture[0].id;
}

void CompMaterial::SetUUIDMesh(uint uuid)
{
	uuid_material = uuid;
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
	if (texture.size() > 0)
	{
		json_object_dotset_number_with_std(object, name + "Num Textures", texture.size());
		json_object_dotset_number_with_std(object, name + "ID Texture", texture.begin()._Ptr->id);
		json_object_dotset_string_with_std(object, name + "Directory Material", texture[0].name.c_str());
	}
}

void CompMaterial::Load(const JSON_Object* object, std::string name)
{
	int num_textrues = json_object_dotget_number_with_std(object, name + "Num Textures");
	if (num_textrues > 0)
	{
		const char* directory = json_object_dotget_string_with_std(object, name + "Directory Material");
		std::string name3 = App->fs->AddDirectorybyType(directory, IMPORT_DIRECTORY_LIBRARY_MATERIALS);
		//App->importer->iMaterial->Load(name3.c_str(), this);

		Texture tex;
		tex.id = json_object_dotget_number_with_std(object, name + "ID Texture");
		AddTexture(tex);
	}
}
