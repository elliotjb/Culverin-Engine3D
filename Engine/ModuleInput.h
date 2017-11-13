#pragma once
#include "Module.h"
#include "Globals.h"
#include "parson.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init(JSON_Object* node);
	//bool Start();
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	//update_status PostUpdate(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

	int GetMouseXGlobal() const
	{
		return mouse_x_global;
	}

	int GetMouseYGlobal() const
	{
		return mouse_y_global;
	}

	int GetMouseXMotionGlobal() const
	{
		return mouse_x_motion_global;
	}

	int GetMouseYMotionGlobal() const
	{
		return mouse_y_motion_global;
	}

	void SetMotionGlobal(int x, int y)
	{
		mouse_x_global = x;
		mouse_y_global = y;
	}

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;


	bool chek = false;

	int mouse_x_global;
	int mouse_y_global;
	int mouse_x_motion_global;
	int mouse_y_motion_global;

public:
	char* dropped_filedir = nullptr;
	bool dropped = false;
};