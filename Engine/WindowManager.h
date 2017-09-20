#pragma once
#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include "Globals.h"
#include "imgui.h"
#include <list>
#include <vector>
#include "MathGeoLib.h"

class WindowManager
{
public:
	WindowManager(bool start_enabled = false)
	{

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


public:


};

#endif // __WINDOWMANAGER_H__ 