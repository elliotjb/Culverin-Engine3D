#ifndef __MODULEOBJECTSUI_H__
#define __MODULEOBJECTSUI_H__

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"

enum window
{
	CREATE,
	SHOW
};

class ModuleObjectsUI : public WindowManager
{
public:

	ModuleObjectsUI();
	virtual ~ModuleObjectsUI();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	void ShowCreateObjects();

	void ShowObjectsinScene();

	void SpecialFunction(const std::string name);

	//void OpenClose();
	//bool IsOpen();


private:

	// INTERSECTION TESTS ------------

	Sphere Sphere_A;
	Sphere Sphere_B;

	bool CreateObjects = false;
	bool ShowObjects = false;

};


#endif