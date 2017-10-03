#pragma once
#ifndef __MODULEOBJECTS_H__
#define __MODULEOBJECTS_H__

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "Primitive.h"
#include "GL3W\include\glew.h"
#include "WindowManager.h"

#define PI 3.1415

class Cylindre
{
public:

	Cylindre();
	Cylindre(const float3 pos, float lenght, float radius);
	~Cylindre();

	void Create();

	void MakeCylinder(float radius, float length, unsigned int numSteps);

	//bool Start();
	//update_status Update(float dt);
	//bool CleanUp();


public:
	float3 pos;
	std::vector<float3> vertex;
	float3 index;
	float lenght;
	float radius;


};


#endif