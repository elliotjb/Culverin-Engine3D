#pragma once
#ifndef __MODULEOBJECTS_H__
#define __MODULEOBJECTS_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "Primitive.h"
#include "GL3W\include\glew.h"
#include "WindowManager.h"

class Cylindre
{
public:

	Cylindre();
	Cylindre(const float3 pos, float lenght, float radius);
	~Cylindre();

	//bool Start();
	//update_status Update(float dt);
	//bool CleanUp();


public:
	float3 pos;
	float lenght;
	float radius;


};


#endif