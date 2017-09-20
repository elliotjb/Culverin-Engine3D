#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include <list>
#include <vector>
#include "MathGeoLib.h"
#include "WindowManager.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = false);
	virtual ~ModuleGUI();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void UpdateWindows(float dt);

	void ShowExampleAppConsole();
	void ShowCreateObjects();
	void ShowObjectsinScene();
	void ShowHardware();
	void ShowConfig();

public:

	// INTERSECTION TESTS ------------
	std::list<Sphere*> list_Sphere;
	std::list<Capsule*> list_Capsule;
	std::list<Plane*> list_Plane;
	std::list<Ray*> list_Ray;
	Sphere Sphere_A;
	Sphere Sphere_B;
	/*Capsule Capsule_A;
	Capsule Capsule_B;
	Line line;
	Plane plane;*/
	bool ShowTest = false;

	std::vector<WindowManager*> winManager;
	// ---------------------------------

	//RANDOM NUMBERS TEST ----------
	
	std::vector<float> floats_list;
	std::vector<int> ints_list;
	//------------------------------
};

#endif