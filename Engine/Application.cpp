#include "Application.h"
#include "parson.h"
#include "PerfTimer.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "SDL\include\SDL.h"
#include "mmgr\mmgr.h"

static int malloc_count;
static void *counted_malloc(size_t size);
static void counted_free(void *ptr);

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio(true);
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	geometry_manager = new ModuleGeometries();
	scene = new Scene();
	console = new Console();
	gui = new ModuleGUI();
	loader = new ModuleLoader();
	importer = new ModuleImporter();
	fs = new ModuleFS();
	textures = new ModuleTextures();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(fs);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(geometry_manager);
	AddModule(console);
	AddModule(scene);
	AddModule(gui);
	AddModule(loader);
	AddModule(importer);
	AddModule(textures);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("config.json");

	configuration = new DockContext();
	
	//In case of error
	if (config_file != nullptr)
	{
		ret = true;

		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config,"Application");
		appName = json_object_get_string(config_node, "App Name");
		orgName = json_object_get_string(config_node, "Org Name");
		maxFPS = json_object_get_number(config_node, "Max FPS");
		vsync = json_object_get_boolean(config_node, "VSYNC");
		SetFpsCap(maxFPS);
		//------------------------------------

		// Call Init() in all modules
		p2List_item<Module*>* item = list_modules.getFirst();

		while (item != NULL && ret == true)
		{
			if (item->data->IsEnabled())
			{
				config_node = json_object_get_object(config, item->data->name.c_str());
				ret = item->data->Init(config_node);
			}
			item = item->next;
		}

		// After all Init calls we call Start() in all modules
		LOG("Application Start --------------");
		item = list_modules.getFirst();

		while (item != NULL && ret == true)
		{
			if (item->data->IsEnabled())
				ret = item->data->Start();
			item = item->next;
		}
		startup_time.Start();
		ms_timer.Start();

	}
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	dt = (float)ms_timer.ReadSec();
	ms_timer.Start();
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Framerate calculations ----------------------
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;


		fps_log[frame_index] = prev_last_sec_frame_count;
		frame_index = (frame_index + 1) % IM_ARRAYSIZE(fps_log);

		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;
	
	ms_log[ms_index] = last_frame_ms;

	//Get all performance data-------------------
	p2List_item<Module*>* item = list_modules.getFirst();

	while (item != NULL)
	{
		if (item->data->IsEnabled())
		{
			item->data->pre_log[ms_index] = item->data->preUpdate_t;
			item->data->up_log[ms_index] = item->data->Update_t;
			item->data->post_log[ms_index] = item->data->postUpdate_t;
		}
		item = item->next;
	}

	ms_index = (ms_index + 1) % IM_ARRAYSIZE(ms_log); //ms_index works for all the logs (same size)


	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	//IMGUI----------------------------------------------------

	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	ImGui_ImplSdlGL3_NewFrame(window->window);

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->Update(dt);
		item = item->next;
	}

	//CONFIG WINDOW -------------------------------------
	//Config();
	//PERFORMANCE WINDOW -----------------------------
	if (showperformance)
	{
		static bool stop_perf = false;
		item = list_modules.getFirst();

		if (!ImGui::Begin("PERFORMANCE", &showperformance, ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
			stop_perf = true;
		}
		else if (stop_perf == false)
		{
			ImGui::Spacing();
			while (item != NULL)
			{
				if (item->data->IsEnabled())
				{
					item->data->ShowPerformance(ms_index);
				}
				item = item->next;
			}
			ImGui::End();
		}
		stop_perf = false;
	}
	//-----------------------------------------------

	ImGui::Render();

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

void Application::Config()
{
	if (showconfig)
	{
		p2List_item<Module*>* item = list_modules.getFirst();
		static bool stop_conf = false;
		bool ret = true;
		item = list_modules.getFirst();

		if (!BeginDock("CONFIGURATION", &showconfig, 0))
		{
			EndDock();
			stop_conf = true;
		}
		configuration->_BeginWorkspace("ConfigurationWindow");
		if (stop_conf == false)
		{
			//ImGui::Spacing();
			//ImGui::Begin
			static bool* temp = NULL;

			if (!configuration->_BeginDock("Application", temp, 0))
			{
				configuration->_EndDock();
			}
			else
			{
				ImGui::Text("App Name:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), appName.c_str());
				ImGui::Text("Organization Name:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), orgName.c_str());
				static int fps = maxFPS;
				ImGui::SliderInt("Max FPS", &fps, 0, 60);
				ImGui::SameLine(); ShowHelpMarker("0 = no framerate cap"); ImGui::SameLine();
				if (ImGui::Button("APPLY"))
				{
					SetFpsCap(fps);
				}
				ImGui::Text("Framerate:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f", fps_log[frame_index - 1]);
				ImGui::PlotHistogram("", fps_log, IM_ARRAYSIZE(fps_log), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
				ImGui::Text("Milliseconds:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f", ms_log[ms_index - 1]);
				ImGui::PlotHistogram("", ms_log, IM_ARRAYSIZE(ms_log), 0, NULL, 0.0f, 50.0f, ImVec2(0, 80));
				ImGui::Checkbox("VSYNC", &vsync); ImGui::SameLine();
				ShowHelpMarker("Restart to apply changes");

				configuration->_EndDock();
			}
			sMStats stats = m_getMemoryStatistics();
			if (!configuration->_BeginDock("Memory Consumption", temp, 0))
			{
				configuration->_EndDock();
			}
			else
			{
				ImGui::BulletText("ACCUMULATED");
				ImGui::Text("- Actual Memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.accumulatedActualMemory);
				ImGui::Text("- Allocated memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.accumulatedAllocUnitCount);
				ImGui::Text("- Reported memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.accumulatedReportedMemory);
				ImGui::Spacing();

				ImGui::BulletText("PEAK");
				ImGui::Text("- Actual Memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.peakActualMemory);
				ImGui::Text("- Allocated memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.peakAllocUnitCount);
				ImGui::Text("- Reported memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.peakReportedMemory);
				ImGui::Spacing();

				ImGui::BulletText("TOTAL");
				ImGui::Text("- Actual memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.totalActualMemory);
				ImGui::Text("- Allocated memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.totalAllocUnitCount);
				ImGui::Text("- Reported memory:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.08f, 1.0f), "%i", stats.totalReportedMemory);


				configuration->_EndDock();
			}

			while (item != NULL && ret == UPDATE_CONTINUE)
			{
				if (item->data->IsEnabled() && item->data->haveConfig)
				{
					static bool* closeButton = NULL;
					if (!configuration->_BeginDock(item->data->name.c_str(), closeButton, ImGuiWindowFlags_NoCollapse))
					{
						configuration->_EndDock();
					}
					else
					{
						ret = item->data->UpdateConfig(dt);
						configuration->_EndDock();
					}
				}

				item = item->next;
			}
			configuration->_EndWorkspace();
			EndDock();
		}
		else
		{
			configuration->_EndWorkspace();
		}
		stop_conf = false;
		//----------------------------------------------
	}
}

bool Application::CleanUp()
{
	bool ret = true;

	//Before Cleaning, save data to config file
	SaveConfig();

	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

bool Application::GetVSYNC()
{
	return vsync;
}

void Application::SetFpsCap(uint fps)
{
	maxFPS = fps;

	if (fps > 0)
	{
		capped_ms = 1000 / fps;
	}
	else
	{
		capped_ms = 0;
	}

}

bool Application::SaveConfig()
{
	bool ret = false;

	LOG("SAVING CONFIG TO FILE -----------------------")

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("config.json");

	if (config_file != nullptr)
	{
		ret = true;

		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Application");

		json_object_set_string(config_node, "App Name", appName.c_str());
		json_object_set_string(config_node, "Org Name", orgName.c_str());
		json_object_set_number(config_node, "Max FPS", maxFPS);
		json_object_set_boolean(config_node, "VSYNC", vsync);


		//Iterate all modules to save each respective info
		p2List_item<Module*>* item = list_modules.getFirst();

		while (item != NULL && ret == true)
		{
			config_node = json_object_get_object(config, item->data->name.c_str());
			ret = item->data->SaveConfig(config_node);
			item = item->next;
		}

		json_serialize_to_file(config_file, "config.json");
	}


	return ret;
}

void Application::ShowHelpMarker(const char * desc, const char * icon)
{
	ImGui::TextDisabled(icon);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}



void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

static void *counted_malloc(size_t size) {
	void *res = malloc(size);
	if (res != NULL) {
		malloc_count++;
	}
	return res;
}

static void counted_free(void *ptr) {
	if (ptr != NULL) {
		malloc_count--;
	}
	free(ptr);
}