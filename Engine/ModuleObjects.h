#pragma once
#ifndef __MODULEOBJECTS_H__
#define __MODULEOBJECTS_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "WindowManager.h"


class ModuleObjects : public WindowManager
{
public:

	ModuleObjects(bool enabled = false);
	virtual ~ModuleObjects();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	void OpenClose_CreateObjects();
	void OpenClose_ShowObjects();
	void ShowCreateObjects();

	void ShowObjectsinScene();

	//void OpenClose();
	//bool IsOpen();


private:

	// INTERSECTION TESTS ------------
	std::list<Sphere*> list_Sphere;
	std::list<Capsule*> list_Capsule;
	std::list<Plane*> list_Plane;
	std::list<Ray*> list_Ray;
	Sphere Sphere_A;
	Sphere Sphere_B;

	bool CreateObjects = false;
	bool ShowObjects = false;

};


#endif