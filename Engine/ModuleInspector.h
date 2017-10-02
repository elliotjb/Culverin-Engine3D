#pragma once
#ifndef __MODULEINSPECTOR_H__
#define __MODULEINSPECTOR_H__

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"


class Inspector : public WindowManager
{
public:

	Inspector();
	virtual ~Inspector();

	bool Start();
	update_status Update(float dt);
	void ShowInspector();
	bool CleanUp();

	//void OpenClose();
	//bool IsOpen();


private:


};


#endif