#pragma once
#include "Globals.h"
#include "parson.h"
#include <string>

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
};