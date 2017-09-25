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

	int		frame_index = 0;
	int		ms_index = 0;

	p2List<Module*> list_modules;

public:

	float  fps_log[70] = { 0 };
	float  ms_log[70] = { 0 };

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;