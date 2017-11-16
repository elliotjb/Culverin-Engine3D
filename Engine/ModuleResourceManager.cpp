#include "ModuleResourceManager.h"
#include "Application.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "ImportMesh.h"
#include "ModuleFS.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"


ModuleResourceManager::ModuleResourceManager(bool start_enabled): Module(start_enabled)
{
	Start_enabled = true;
	preUpdate_enabled = true;

	name = "Resources Manager";
}

ModuleResourceManager::~ModuleResourceManager()
{
	std::map<uint, Resource*>::iterator it = resources.begin();
	for (int i = 0; i < resources.size(); i++)
	{
		RELEASE(it->second);
		it++;
	}
	resources.clear();
}

bool ModuleResourceManager::Start()
{
	perf_timer.Start();

	CreateResourceCube();
	//Load();

	Start_t = perf_timer.ReadMs();
	return true;
}


update_status ModuleResourceManager::PreUpdate(float dt)
{
	perf_timer.Start();

	if (App->input->dropped)
	{
		allfilesprepared = true;
	}
	if (allfilesprepared)
	{
		if (App->input->dropped == false)
		{
			nowImport = true;
		}
	}
	if(nowImport)
	{
		ImportFile(App->input->dropedfiles);
		App->input->dropedfiles.clear();
		nowImport = false;
		allfilesprepared = false;
	}

	preUpdate_t = perf_timer.ReadMs();
	return UPDATE_CONTINUE;
}

//update_status ModuleResourceManager::Update(float dt)
//{
//	return UPDATE_CONTINUE;
//}

bool ModuleResourceManager::CleanUp()
{
	//Save();
	return true;
}

void ModuleResourceManager::ImportFile(std::list<const char*>& file)
{
	std::list<const char*>::iterator& it = file.begin();
	for (int i = 0; i < file.size(); i++)
	{
		// Get Type file
		Resource::Type dropped_File_type = Resource::Type::UNKNOWN;
		dropped_File_type = CheckFileType(it._Ptr->_Myval);

		if (dropped_File_type != Resource::Type::UNKNOWN)
		{
			if (App->importer->Import(it._Ptr->_Myval, dropped_File_type))
			{
				// Copy file to Specify folder in Assets (This folder is the folder active)
				App->fs->CopyFileToAssets(it._Ptr->_Myval, ((Project*)App->gui->winManager[WindowName::PROJECT])->GetDirectory());
				((Project*)App->gui->winManager[WindowName::PROJECT])->UpdateNow();
			}
		}
		else
		{
			LOG("[error] This file: %s with this format %s is incorrect!", App->fs->FixName_directory(it._Ptr->_Myval).c_str(), App->fs->GetExtension(it._Ptr->_Myval));
		}
		it++;
	}
}

Resource* ModuleResourceManager::CreateNewResource(Resource::Type type, uint uuid)
{
	Resource* ret = nullptr;
	uint uid;
	if(uuid == 0)
		uid = App->random->Int();
	else
		uid = uuid;

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

Resource* ModuleResourceManager::GetResource(const char* material)
{
	std::map<uint, Resource*>::iterator it = resources.begin();
	for (int i = 0; i < resources.size(); i++)
	{
		if (strcmp(it->second->name, material) == 0)
		{
			return it->second;
		}
		it++;
	}
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
	App->importer->iMesh->Import(8, 36, 0, indices, vertices, 1); // 1 == Cube
}

Resource*  ModuleResourceManager::ShowResources(bool& active, Resource::Type type)
{
	const char* nameWindow = nullptr;
	const char* subname = nullptr;
	if (type == Resource::Type::MESH)
	{
		nameWindow = "Select Mesh";
		subname = "All Meshes";
	}
	else if (type == Resource::Type::MATERIAL)
	{
		nameWindow = "Select Material";
		subname = "All Materials:";
	}
	if (!ImGui::Begin(nameWindow, &active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders)) //TODO ELLIOT CLOSE Windows example
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text(subname);
		ImGui::Spacing();
		std::map<uint, Resource*>::iterator it = resources.begin();
		for (int i = 0; i < resources.size(); i++)
		{
			ImGui::PushID(i);
			if (type == it->second->GetType())
			{
				ImGui::ButtonEx(it->second->name);
				if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
				{
					if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringWindow())
					{
						ImGui::PopID();
						ImGui::End();
						return GetResource(it->second->GetUUID());
					}
				}
			}
			it++;
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
		std::map<uint, Resource*>::iterator it = resources.begin();
		for (uint i = 0; i < resources.size(); i++)
		{
			std::string name = "Resource " + std::to_string(i);
			name += ".";
			json_object_dotset_number_with_std(config_node, name + "UID", it->second->GetUUID());
			json_object_dotset_number_with_std(config_node, name + "Type", (int)it->second->GetType());
			json_object_dotset_number_with_std(config_node, name + "Directory Mesh UID", it->second->uuid_mesh);
			json_object_dotset_string_with_std(config_node, name + "Name", it->second->name);
			it++;
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
					uint uid = json_object_dotget_number_with_std(config_node, name + "UID");
					ResourceMesh* mesh = (ResourceMesh*)CreateNewResource(type, uid);
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
