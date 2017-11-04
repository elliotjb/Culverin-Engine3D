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
#include "Scene.h"
#include "ModuleConsole.h"
#include "ModuleGUI.h"
#include "ModuleGeometries.h" //TODO JORDI OÑA AND ELLIOT forward declaration
#include "ModuleImporter.h"
#include "ModuleTextures.h"
#include "ModuleFS.h"
#include "parson.h"
#include <vector>
#include <string>
#include "GL3W/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ImGui/imgui_dock_v2.h"
#include "Algorithm/Random/LCG.h"
#include "Math/float4.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleGeometries* geometry_manager;
	Scene* scene;
	Console* console;
	ModuleGUI* gui;
	ModuleImporter* importer;
	ModuleFS* fs;
	ModuleTextures* textures;

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
	void Config();
	bool CleanUp();

	bool GetVSYNC();
	void SetFpsCap(uint);

	bool SaveConfig();

	static void ShowHelpMarker(const char* desc, const char* icon = "(?)");
	char* GetCharfromConstChar(const char* name);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	std::string appName;
	std::string orgName;
	uint maxFPS = 0;
	bool vsync = true;

	DockContext* configuration = nullptr;

public:
	LCG* random = nullptr;
	bool showconfig = false;
	bool showperformance = false;
	float4 SceneDock = { 0, 0, 0, 0 };
};

extern Application* App;