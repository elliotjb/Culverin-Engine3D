#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include <list>
#include <vector>
#include "MathGeoLib.h"
#include "WindowManager.h"
#include "SDL\include\SDL.h"

enum WindowName
{
	//Names of Windows (Modules)
	HARDWARE,
	CREATEOBJETCS,
	INSPECTOR,
	HIERARCHY,
	SCENEWORLD
};

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool start_enabled = false);
	virtual ~ModuleGUI();

	bool Init(JSON_Object* node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void UpdateWindows(float dt);

	void ShowExampleAppConsole();
	void ShowHardware();
	void ShowInfoMouse(bool* active);
	void ShowWindowExit(bool* active);
	void ShowPerformance(int ms_index);
	//void ShowConfig();
	void LoadDocks();
	void DockTest();

public:


	/*Capsule Capsule_A;
	Capsule Capsule_B;
	Line line;
	Plane plane;*/
	bool ShowTest = false;
	bool isSaved = false;
	std::vector<WindowManager*> winManager;

	//RANDOM NUMBERS TEST ----------
	std::vector<float> floats_list;
	std::vector<int> ints_list;
	//------------------------------

private:
	//WINDOW EXIT -------------
	bool gameSave = false;
	bool reposition_exit = true;

	int menu_height = 0;
	bool dock_test = false;

};
#endif