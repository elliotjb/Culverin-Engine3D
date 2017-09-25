#pragma once
#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "Scene.h"
#include "ModuleConsole.h"
#include "ModuleGUI.h"
#include "parson.h"
#include <vector>
#include "GL3W\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl\GL.h>
#include <gl\GLU.h>


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModulePlayer* player;
	Scene* scene;
	Console* console;
	ModuleGUI* gui;


private:

	Timer	ms_timer;
	uint64	frame_count = 0;
	Timer	startup_time;
	Timer	frame_time;
	Timer	last_sec_frame_time;
	uint32	last_sec_frame_count = 0;
	uint32	prev_last_sec_frame_count = 0;
	float	dt = 0.0f;
	int		capped_ms = -1;

	p2List<Module*> list_modules;

public:

	int		frame_index = 0;
	int		ms_index = 0;
	float	fps_log[70] = { 0 };
	float	ms_log[70] = { 0 };

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	static void ShowHelpMarker(const char* desc, const char* icon = "(?)");

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();



public:
	bool showconfig = false;
};

extern Application* App;