#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "p2Point.h"
#include "p2List.h"
#include "MathGeoLib.h"
#include "p2DynArray.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = false);
	virtual ~ModuleGUI();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowExampleAppConsole();

public:

	// INTERSECTION TESTS ------------
	p2List<Sphere> list_Sphere;
	p2List<Capsule> list_Capsule;
	p2List<Plane> list_Plane;
	p2List<Ray> list_Ray;
	Sphere Sphere_A;
	Sphere Sphere_B;
	/*Capsule Capsule_A;
	Capsule Capsule_B;
	Line line;
	Plane plane;*/
	bool ShowTest = false;
	// ---------------------------------

	//RANDOM NUMBERS TEST ----------
	
	p2DynArray<float> floats_list;
	p2DynArray<int> ints_list;
	//------------------------------
};

#endif