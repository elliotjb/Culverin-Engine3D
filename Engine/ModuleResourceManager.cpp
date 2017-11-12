#include "ModuleResourceManager.h"
#include "Application.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "ImportMesh.h"


ModuleResourceManager::ModuleResourceManager(bool start_enabled): Module(start_enabled)
{
	name = "Resources Manager";
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Start()
{
	CreateResourceCube();

	//Load();
	return true;
}


update_status ModuleResourceManager::PreUpdate(float dt)
{
	if (App->input->dropped)
	{
		ImportFile(App->input->dropped_filedir);

		App->input->dropped = false;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	Save();
	return true;
}

void ModuleResourceManager::ImportFile(const char* file)
{
	// Get Type file
	Resource::Type dropped_File_type = Resource::Type::UNKNOWN;
	dropped_File_type = CheckFileType(App->input->dropped_filedir);

	if (dropped_File_type != Resource::Type::UNKNOWN)
	{
		App->importer->Import(App->input->dropped_filedir, dropped_File_type);

		// Copy file to Specify folder in Assets (This folder is the folder active)
		App->fs->CopyFileToAssets(App->input->dropped_filedir, ((Project*)App->gui->winManager[WindowName::PROJECT])->GetDirectory());
		((Project*)App->gui->winManager[WindowName::PROJECT])->UpdateNow();
	}
	else
	{
		LOG("[error] This file: %s with this format %s is incorrect!", App->fs->FixName_directory(file).c_str(), App->fs->GetExtension(file));
	}
}

Resource* ModuleResourceManager::CreateNewResource(Resource::Type type)
{
	Resource* ret = nullptr;
	uint uid = last_uid++;

	switch (type) 
	{
	case Resource::MATERIAL: ret = (Resource*) new ResourceMaterial(uid); break;
	case Resource::MESH: ret = (Resource*) new ResourceMesh(uid); break;
	}
	if (ret != nullptr)
		resources[uid] = ret;
	return ret;
}

Resource* ModuleResourceManager::GetResource(uint id)
{
	std::map<uint, Resource*>::iterator it = resources.find(id);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

Resource::Type ModuleResourceManager::CheckFileType(const char* filedir)
{
	if (filedir != nullptr)
	{
		std::string file_type;
		std::string path(filedir);

		size_t extension_pos = path.find_last_of(".");

		file_type = path.substr(extension_pos + 1);

		//Set lowercase the extension to normalize it
		for (std::string::iterator it = file_type.begin(); it != file_type.end(); it++)
		{
			*it = tolower(*it);
		}

		if (file_type == "png" || file_type == "jpg" || file_type == "dds")
		{
			return Resource::Type::MATERIAL;
		}
		else if (file_type == "fbx" || file_type == "obj" || file_type == "FBX")
		{
			return Resource::Type::MESH;
		}
		else
		{
			return Resource::Type::UNKNOWN;
		}
	}
}

void ModuleResourceManager::Init_IndexVertex(float3* vertex_triangulate, uint num_index, std::vector<uint>& indices, std::vector<float3>& vertices)
{
	bool temp = false;
	std::vector<float3> all_index;
	uint size = 0;

	for (int i = 0; i < num_index; i++)
	{
		temp = false;
		size = all_index.size();

		for (int j = 0; j < size; j++)
		{
			if (all_index[j] == vertex_triangulate[i])
			{
				indices.push_back(j);
				temp = true;
			}
		}

		if (temp == false)
		{
			all_index.push_back(vertex_triangulate[i]);
			indices.push_back(all_index.size() - 1);
			vertices.push_back(vertex_triangulate[i]);
		}
	}
}

void ModuleResourceManager::CreateResourceCube()
{
	OBB* box = new OBB();
	box->pos = float3::zero;
	box->r = float3::one;
	box->axis[0] = float3(1, 0, 0);
	box->axis[1] = float3(0, 1, 0);
	box->axis[2] = float3(0, 0, 1);

	AABB* bounding_box;
	bounding_box = new AABB(*box);
	float3* vertices_array = new float3[36];

	bounding_box->Triangulate(1, 1, 1, vertices_array, NULL, NULL, false);
	std::vector<uint> indices;
	std::vector<float3> vertices;
	Init_IndexVertex(vertices_array, 36, indices, vertices);
	App->importer->iMesh->Import(8, 36, 0, indices, vertices);
}

Resource*  ModuleResourceManager::ShowResources(bool& active)
{
	if (!ImGui::Begin("Select Mesh", &active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders)) //TODO ELLIOT CLOSE Windows example
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("SDL Version: ");
		for (int i = 0; i < resources.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::ButtonEx(resources[i]->name);
			if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
			{
				if (ImGui::IsMouseDoubleClicked(0))
				{
					ImGui::PopID();
					ImGui::End();
					return GetResource(i);
				}
			}
			ImGui::PopID();
		}
		ImGui::End();
	}
	return nullptr;
}

void ModuleResourceManager::Save()
{
	LOG("----- SAVING RESOURCES -----");

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("Resources.json");

	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Resources");
		json_object_clear(config_node);
		json_object_dotset_number_with_std(config_node, "Info.Number of Resources", resources.size());

		// Update Resoruces
		for (uint i = 0; i < resources.size(); i++)
		{
			std::string name = "Resource " + std::to_string(i);
			name += ".";
			Resource* resource = GetResource(i);
			json_object_dotset_number_with_std(config_node, name + "Type", (int)resource->GetType());
			json_object_dotset_number_with_std(config_node, name + "Directory Mesh UID", resource->uuid_mesh);
			json_object_dotset_string_with_std(config_node, name + "Name", resource->name);
		}
	}
	json_serialize_to_file(config_file, "Resources.json");
}

void ModuleResourceManager::Load()
{
	LOG("----- LOADING RESOURCES -----");

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("Resources.json");
	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Resources");
		int NumberResources = json_object_dotget_number(config_node, "Info.Number of Resources");
		if (NumberResources > 0)
		{
			for (int i = 0; i < NumberResources; i++)
			{
				std::string name = "Resource " + std::to_string(i);
				name += ".";
				Resource::Type type = (Resource::Type)(int)json_object_dotget_number_with_std(config_node, name + "Type");
				switch (type)
				{
				case Resource::Type::MESH:
				{
					ResourceMesh* mesh = (ResourceMesh*)CreateNewResource(type);
					mesh->name = App->GetCharfromConstChar(json_object_dotget_string_with_std(config_node, name + "Name"));
					mesh->uuid_mesh = json_object_dotget_number_with_std(config_node, name + "Directory Mesh UID");
					break;
				}
				case Resource::Type::MATERIAL:
				{
					ResourceMaterial* material = (ResourceMaterial*)CreateNewResource(type);
					break;
				}
				case Resource::Type::UNKNOWN:
				{
					LOG("Error load resoruce");
					break;
				}
				}
			}
		}
	}
}
