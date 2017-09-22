#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include <list>
#include <vector>
#include "MathGeoLib.h"
#include "WindowManager.h"

enum WindowName
{
	HARDWARE,
	CREATEOBJETCS
};

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool start_enabled = false);
	virtual ~ModuleGUI();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void UpdateWindows(float dt);

	void ShowExampleAppConsole();
	void ShowHardware();
	void ShowConfig();

public:


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