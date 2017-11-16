#pragma once
#include "CompMaterial.h"
#include "Color.h"
#include "GAmeObject.h"
#include "parson.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ImportMaterial.h"
#include "Scene.h"

CompMaterial::CompMaterial(Comp_Type t, GameObject* parent): Component(t, parent)
{
	nameComponent = "Material";
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

void CompMaterial::ShowOptions()
{
	// Reset Values Button -------------------------------------------
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
	ImGui::SameLine(ImGui::GetWindowWidth() - 26);
	if (ImGui::ImageButton((ImTextureID*)App->scene->icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
	{
		// Open Options
	}
	ImGui::PopStyleVar();

	//ImGui::MenuItem("CREATE", NULL, false, false);
	if (ImGui::MenuItem("Reset"))
	{
		// Not implmented yet.
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Move to Front", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move to Back", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Remove Component"))
	{
		toDelete = true;
	}
	if (ImGui::MenuItem("Move Up", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move Down", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Copy Component"))
	{
		// Component* Copy = this;
	}
	if (ImGui::MenuItem("Paste Component", NULL, false, false))
	{
		//parent->AddComponent(App->scene->copyComponent->GetType())
		// Create contructor Component Copy or add funtion to add info
	}
}

void CompMaterial::ShowInspectorInfo()
{
	ImGui::ColorEdit3("", (float*)&color);

	ImGui::TreePop();
}

void CompMaterial::Save(JSON_Object* object, std::string name) const
{
	json_object_dotset_number_with_std(object, name + "Type", C_MATERIAL);
	float4 tempColor = { color.r, color.g, color.b, color.a };
	App->fs->json_array_dotset_float4(object, name + "Color", tempColor);
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
	float4 tempColor = App->fs->json_array_dotget_float4_string(object, name + "Color");
	color.Set(tempColor.x, tempColor.y, tempColor.z, tempColor.w);
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
