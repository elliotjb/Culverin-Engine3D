#pragma once
#include "Module.h"
#include "Globals.h"
#include "BaseGeometry.h"
#include <vector>

class BaseGeometry;

class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(bool start_enabled = true);
	virtual ~ModuleFBXLoader();

	bool Start();
	update_status Update(float dt);
	update_status postUpdate(float dt);
	bool CleanUp();

	BaseGeometry* LoadMesh(const char* filename);
	
private:

	BaseGeometry* m;
};
