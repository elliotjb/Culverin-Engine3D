#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "parson.h"
#include "GL3W\include\glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>


#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* node);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	update_status UpdateConfig(float dt);
	bool SaveConfig(JSON_Object* node);

	void ShowPerformance(int ms_index);

	bool CleanUp();

	void OnResize(int width, int height);
	
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool depth_test = false;
	bool cull_face = false;
	bool lightning = false;
	bool color_material = false;
	bool texture_2d = false;
	bool wireframe = false;
	bool axis = true;
	bool smooth = true;
	bool fog_active = false;
	GLfloat fog_density = 0;
};