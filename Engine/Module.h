#pragma once
#include "Globals.h"
#include "parson.h"
#include <string>
#include "PerfTimer.h"

class Application;
struct PhysBody3D;

class Module
{
public:

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* node) 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status UpdateConfig(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool SaveConfig(JSON_Object* node)
	{
		return true;
	}

	virtual void ShowPerformance(int ms_index) {}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	bool IsEnabled() const { return enabled; }

	void Enable()
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}
	void Disable_whitout()
	{
		if (enabled == true)
		{
			enabled = false;
		}
	}

public:

	std::string	name;
	bool enabled = true;
	bool haveConfig = false;

	PerfTimer perf_timer;

	float ms_index = 0;
	float pre_log[70] = { 0 };
	float up_log[70] = { 0 };
	float post_log[70] = { 0 };

	float preUpdate_t = 0;
	float Update_t = 0;
	float postUpdate_t = 0;


};