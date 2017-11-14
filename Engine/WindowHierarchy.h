#pragma once
#ifndef __MODULEHIERARCHY_H__
#define __MODULEHIERARCHY_H__

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"

class GameObject;

class Hierarchy : public WindowManager
{
public:

	Hierarchy();
	virtual ~Hierarchy();

	bool Start();
	update_status Update(float dt);
	void ShowHierarchy();
	void ShowOptions();
	void SetGameObjecttoDelete(GameObject* todelete);
	void ChangeShowConfirmDelete();
	bool CleanUp();

public:
	bool showconfirmDelete = true;
private:
	std::string model_name = "";
	bool haveModel = false;
	GameObject* toDelete = nullptr;
	bool wait_toSelect = false;
	bool dont_ask_me_next_time = false;

};


#endif //__MODULEHIERARCHY_H__