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

	void CreateSphere(Sphere_p* sphere, bool isKynematic);


private:

	std::vector<Sphere_object> spheres;
	std::list<PhysBody3D*> Body_spheres;

};


#endif