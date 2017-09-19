#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"


class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = false);
	virtual ~ModuleGUI();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowExampleAppConsole();

};

#endif