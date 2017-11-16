#pragma once
#include "CompMaterial.h"
#include "Color.h"
#include "GAmeObject.h"
#include "parson.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ImportMaterial.h"
#include "ResourceMaterial.h"
#include "Scene.h"

CompMaterial::CompMaterial(Comp_Type t, GameObject* parent): Component(t, parent)
{
	nameComponent = "Material";
	uid = App->random->Int();
	color = White;
}

CompMaterial::~CompMaterial()
{
	if (resourceMaterial != nullptr)
	{
		resourceMaterial->NumGameObjectsUseMe--;
	}
	resourceMaterial = nullptr;
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

//void CompMaterial::SetTexture(std::vector<Texture> textures)
//{
//	texture = textures;
//}
//
//void CompMaterial::AddTexture(const Texture tex)
//{
//	texture.push_back(tex);
//}

uint CompMaterial::GetTextureID()
{
	//if(texture.size() > 0)
	//	return texture[0].id;
	if (resourceMaterial != nullptr)
	{
		return resourceMaterial->GetTextureID();
	}
	return 0;
}

void CompMaterial::SetUUIDMesh(uint uuid)
{
	uuid_material = uuid;
}

void CompMaterial::ShowOptions()
{
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
	// Reset Values Button -------------------------------------------
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
	ImGui::SameLine(ImGui::GetWindowWidth() - 26);
	if (ImGui::ImageButton((ImTextureID*)App->scene->icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
	{
		ImGui::OpenPopup("OptionsMaterial");
	}
	// Button Options --------------------------------------
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
	if (ImGui::BeginPopup("OptionsMaterial"))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
		if (ImGui::Button("Reset Material"))
		{
			resourceMaterial = nullptr;
			ImGui::CloseCurrentPopup();
		}
		ImGui::Separator();
		if (ImGui::Button("Select Mesh..."))
		{
			selectMaterial = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::ColorEdit3("", (float*)&color);

	if (resourceMaterial != nullptr)
	{
		ImGui::Text("Name:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%s", resourceMaterial->name);

		//ImGui::Checkbox("Render", &render);
	}
	if(resourceMaterial == nullptr || selectMaterial)
	{
		if (resourceMaterial == nullptr)
		{
			if (ImGui::Button("Select Material..."))
			{
				selectMaterial = true;
			}
		}
		if (selectMaterial)
		{
			ResourceMaterial* temp = (ResourceMaterial*)App->resource_manager->ShowResources(selectMaterial, Resource::Type::MATERIAL);
			if (temp != nullptr)
			{
				resourceMaterial = temp;
				if (resourceMaterial->IsLoadedToMemory() == false)
				{
					App->importer->iMaterial->LoadResource(std::to_string(resourceMaterial->uuid_mesh).c_str(), resourceMaterial);
				}
				Enable();
			}
		}
	}

	ImGui::TreePop();
}

void CompMaterial::Save(JSON_Object* object, std::string name) const
{
	json_object_dotset_number_with_std(object, name + "Type", C_MATERIAL);
	float4 tempColor = { color.r, color.g, color.b, color.a };
	App->fs->json_array_dotset_float4(object, name + "Color", tempColor);
	json_object_dotset_number_with_std(object, name + "UUID", uid);
	if (resourceMaterial != nullptr)
	{
		json_object_dotset_number_with_std(object, name + "Resource Material ID", resourceMaterial->GetUUID());
	}
	else
	{
		json_object_dotset_number_with_std(object, name + "Resource Material ID", 0);
	}
}

void CompMaterial::Load(const JSON_Object* object, std::string name)
{
	float4 tempColor = App->fs->json_array_dotget_float4_string(object, name + "Color");
	color.Set(tempColor.x, tempColor.y, tempColor.z, tempColor.w);
	uid = json_object_dotget_number_with_std(object, name + "UUID");
	uint resourceID = json_object_dotget_number_with_std(object, name + "Resource Material ID");
	if (resourceID > 0)
	{
		resourceMaterial = (ResourceMaterial*)App->resource_manager->GetResource(resourceID);
		resourceMaterial->NumGameObjectsUseMe++;
		//TODO ELLIOT -> LOAD MESH
		//const char* directory = App->GetCharfromConstChar(std::to_string(uuid_mesh).c_str());
		if (resourceMaterial->IsLoadedToMemory() == false)
		{
			App->importer->iMaterial->LoadResource(std::to_string(resourceMaterial->uuid_mesh).c_str(), resourceMaterial);
		}
	}
	Enable();
}
