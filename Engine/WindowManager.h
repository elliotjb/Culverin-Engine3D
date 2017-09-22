#pragma once
#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include "Globals.h"
#include "imgui.h"
#include <list>
#include <vector>
#include "MathGeoLib.h"

class ModuleObjects;

class WindowManager
{
public:
	WindowManager(bool start_enabled = false)
	{
		active = start_enabled;
	}
	virtual ~WindowManager()
	{

	}

	virtual bool Start()
	{
		return true;
	}
	virtual update_status Update(float dt)
	{

		return update_status();
	}
	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OpenClose()
	{
		active = !active;
	}

	virtual bool IsActive()
	{
		return active;
	}

public:
	bool active = false;

	// or std::vector<bool> active; YES
	ModuleObjects* obj = nullptr;

};

#endif // __WINDOWMANAGER_H__ 