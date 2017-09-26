#pragma once
#ifndef __MODULEOBJECTS_H__
#define __MODULEOBJECTS_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "Primitive.h"
#include "WindowManager.h"

struct Sphere_object
{
	Sphere_p sphere;
	bool isKynematic = false;
};

class ModuleObjects : public Module
{
public:

	ModuleObjects(bool start_enabled = true);
	virtual ~ModuleObjects();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void CreateSphere(Sphere_p* sphere, bool isKynematic = false);
	void ShowAxis(bool axis);
	void ShowWire(bool wire);

public:

	std::vector<Sphere_object> spheres;
	std::list<PhysBody3D*> Body_spheres;

	std::list<Sphere*> math_Sphere;
	std::list<Capsule*> math_Capsule;
	std::list<Plane*> math_Plane;
	std::list<Ray*> math_Ray;

};


#endif