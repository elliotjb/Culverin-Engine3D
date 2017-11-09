#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "Scene.h"
#include "ModuleConsole.h"
#include "ModuleGUI.h"			//TODO JORDI OÑA AND ELLIOT forward declaration
#include "ModuleImporter.h"
#include "ModuleTextures.h"
#include "ModuleFS.h"
#include "ModuleResourceManager.h"
#include "parson.h"
#include "GL3W/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "ImGui/imgui_dock_v2.h"
#include "Algorithm/Random/LCG.h"
#include "Math/float4.h"
#include <vector>
#include <string>
#include <gl/GL.h>
#include <gl/GLU.h>


class JSONSerialization;

enum EngineState
{
	PLAY = 0,
	PAUSE,
	STOP,
	PLAYFRAME
};

struct GameClock
{
	float gameStart_time = 0.0f; 
	float timeScale = 1.0f; 
	uint64 frame_count = 0; 

	bool prepare_frame = false;
	bool play_frame = false;
};

struct RealTimeClock
{
	Timer engineStart_time;
	Timer last_sec_frame_time;
	Timer frame_time;
	Timer ms_timer;

	uint64 frame_count = 0;
	uint32 last_sec_frame_count = 0;
	uint32 prev_last_sec_frame_count = 0;
	uint32 last_frame_ms = 0;
	float dt = 0.0f;
	int capped_ms = -1;
};

class Application
{
public:

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

	void SetState(EngineState state);

	void WantToSave();
	void WantToLoad();

	void ChangeCamera(const char* window);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	// MODULES --------------------------
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	Scene* scene;
	Console* console;
	ModuleGUI* gui;
	ModuleImporter* importer;
	ModuleFS* fs;
	ModuleTextures* textures;
	ModuleResourceManager* resource_manager;

private:
	std::list<Module*> list_modules;
	// ----------------------------------

	// TIME -----------------------------
public:
	RealTimeClock realTime;
	GameClock gameTime;
	EngineState engineState = EngineState::STOP;

	int		frame_index = 0;
	int		ms_index = 0;
	float	fps_log[70] = { 0 };
	float	ms_log[70] = { 0 };

	uint maxFPS = 0;
	bool vsync = true;
	// ----------------------------------

public:
	LCG* random = nullptr;
	bool showconfig = false;
	bool showperformance = false;
	float4 SceneDock = { 0, 0, 0, 0 };

	//Use for Serialization scene or prefabs 
	JSONSerialization* Json_seria = nullptr;

private:
	std::string appName;
	std::string orgName;

	bool want_to_save = false;
	bool want_to_load = false;

	bool change_to_scene = false;
	bool change_to_game = false;

	DockContext* configuration = nullptr;
};

extern Application* App;