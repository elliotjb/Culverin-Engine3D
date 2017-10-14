#pragma once
#include "Module.h"
#include "Globals.h"
#include "GL3W/include/glew.h"

class BaseObject;

class ModuleTextures : public Module
{
public:

	ModuleTextures(bool start_enabled = true);
	virtual ~ModuleTextures();

	bool Init(JSON_Object* node);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	GLuint LoadTexture(const char* filename);
	void SetTexPath(char* path, BaseObject* obj);

private:

};