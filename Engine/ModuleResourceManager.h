#pragma once
#include "Module.h"
#include "Resource_.h"
#include <map>
#include <vector>
#include "Math\float3.h"

struct Vertex;

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool start_enabled = true);
	virtual ~ModuleResourceManager();

	bool Start();
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	bool CleanUp();

	void ImportFile(const char* file);
	Resource* CreateNewResource(Resource::Type type, uint uuid = 0);
	Resource* GetResource(uint id);
	Resource::Type CheckFileType(const char* filedir);

	void Init_IndexVertex(float3* vertex_triangulate, uint num_index, std::vector<uint>& indices, std::vector<float3>& vertices);
	void CreateResourceCube();

	Resource* ShowResources(bool& active);

	void Save();
	void Load();

private:
	std::map<uint, Resource*> resources;
};