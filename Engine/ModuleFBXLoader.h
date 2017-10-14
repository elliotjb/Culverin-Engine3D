#pragma once
#include "Module.h"
#include "Globals.h"
#include "BaseGeometry.h"
#include <vector>
#include <string>
class BaseGeometry;

enum FileType
{
	F_UNKNOWN = -1,
	F_MODEL,
	F_TEXTURE
};

class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(bool start_enabled = true);
	virtual ~ModuleFBXLoader();

	bool Start();
	update_status Update(float dt);
	//update_status postUpdate(float dt);
	bool CleanUp();

	FileType CheckFileType(char* filedir);

	BaseGeometry* LoadMesh(const char* filename);
	void SetInfoToLoadTex(int m, int t, int t_id);

private:
	BaseGeometry* m;
	FileType dropped_Filetype = F_UNKNOWN;

	int mesh_index = -1;
	int tex_index = -1;
	int tex_id = -1;
};
