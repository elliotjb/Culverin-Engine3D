#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleTextures : public Module
{
public:

	ModuleTextures(bool start_enabled = true);
	virtual ~ModuleTextures();

	bool Start();
	bool CleanUp();

	bool LoadTexture(const char* filename);

private:

};