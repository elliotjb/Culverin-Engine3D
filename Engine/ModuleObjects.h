#pragma once
#ifndef __MODULEOBJECTS_H__
#define __MODULEOBJECTS_H__

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "Primitive.h"
#include "GL3W\include\glew.h"
#include "WindowManager.h"

class BaseObject;
class Mesh;

class ModuleObjects : public Module
{
public:

	ModuleObjects(bool start_enabled = true);
	virtual ~ModuleObjects();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowPerformance(int ms_index);

	//OBJECT CREATION--------------------
	bool Index_vert(float3* vertex_triangulate, uint num_vertex, Mesh* obj);

	void CreateCube(float3 pos, float3 size, bool isKynematic = false, Color color = White);
	void CreateSphere(float3 pos, float radius, uint definition = 1, bool isKynematic = false, Color color = White);
	//-----------------------------------

	void ShowAxis(bool axis);
	void ShowWire(bool wire);

public:

	std::vector<BaseObject*> objects;
	//std::vector<_Cube*> cubes;
	//std::vector<_Sphere*> spheres;

	// Math Objects -------------------
	std::list<Sphere*> math_Sphere;
	std::list<Capsule*> math_Capsule;
	std::list<Plane*> math_Plane;
	std::list<Ray*> math_Ray;
	// --------------------------------

	bool wireframe_mode = false;
	uint count = 0;
};


#endif