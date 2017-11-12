#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "parson.h"
#include "GL3W\include\glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#define MAX_LIGHTS 8

class CompCamera;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	update_status PostUpdate(float dt);
	update_status UpdateConfig(float dt);
	bool SaveConfig(JSON_Object* node);

	bool CleanUp();

	void SetActiveCamera(CompCamera* cam);
	void SetSceneCamera(CompCamera* cam);
	void SetGameCamera(CompCamera* cam);

	void UpdateProjection(CompCamera* cam);

	void OnResize(int width, int height);
	
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CompCamera* active_camera = nullptr;
	CompCamera* scene_camera = nullptr;
	CompCamera* game_camera = nullptr;

	bool depth_test = false;
	bool cull_face = false;
	bool lighting = false;
	bool color_material = false;
	bool texture_2d = false;
	bool wireframe = false;
	bool smooth = true;
	bool fog_active = false;
	bool normals = false;
	bool bounding_box = false;

	GLfloat fog_density = 0;
};