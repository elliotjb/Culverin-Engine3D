#pragma once
#include "Module.h"
#include "Globals.h"
#include "BaseObject.h"
#include <vector>

class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(bool start_enabled = true);
	virtual ~ModuleFBXLoader();

	bool Start();
	update_status Update(float dt);
	update_status postUpdate(float dt);
	bool CleanUp();

	bool LoadMesh(const char* filename);
	
private:

	BaseObject* m;
	std::vector<BaseObject*> meshes;
};
