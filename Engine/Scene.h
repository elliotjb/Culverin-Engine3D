#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "imgui.h"
#include "MathGeoLib.h"

/*
#include "GL3W\include\glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
*/

class Scene : public Module
{
public:
	Scene(bool start_enabled = false);
	virtual ~Scene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:



};