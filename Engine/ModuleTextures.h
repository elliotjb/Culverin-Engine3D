#pragma once
#include "Module.h"
#include "Globals.h"
#include "GL3W/include/glew.h"


class ModuleTextures : public Module
{
public:

	ModuleTextures(bool start_enabled = true);
	virtual ~ModuleTextures();

	bool Start();
	bool CleanUp();

	GLuint LoadTexture(const char* filename);

private:

};