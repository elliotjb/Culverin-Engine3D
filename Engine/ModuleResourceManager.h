#pragma once
#include "Module.h"
#include "Resource_.h"
#include <map>
#include <vector>
#include <list>
#include "Math\float3.h"

struct Vertex;

struct ReImport
{
	uint uuid = 0;
	const char* directoryObj = nullptr;
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool start_enabled = true);
	virtual ~ModuleResourceManager();

	bool Start();
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	bool CleanUp();

	void ImportFile(std::list<const char*>& file);
	Resource* CreateNewResource(Resource::Type type, uint uuid = 0);
	Resource* GetResource(uint id);
	Resource* GetResource(const char* material); //Only Use in ImportMesh -> Add recourceMaterial
	Resource::Type CheckFileType(const char* filedir);

	void Init_IndexVertex(float3* vertex_triangulate, uint num_index, std::vector<uint>& indices, std::vector<float3>& vertices);
	void CreateResourceCube();

	Resource* ShowResources(bool& active, Resource::Type type);

	void Save();
	void Load();

public:
	std::vector<ReImport> resourcesToReimport;

private:
	std::map<uint, Resource*> resources;
	bool allfilesprepared = false;
	bool nowImport = false;
};