#include "Application.h"
#include "parson.h"
#include "PerfTimer.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "SDL\include\SDL.h"

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
	player = new ModulePlayer();
	scene = new Scene();
	console = new Console();
	gui = new ModuleGUI();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(gui);
	AddModule(console);
	AddModule(scene);
	AddModule(player);

	player->Disable_whitout();

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

	/*JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* app_config;

	config_file = json_parse_file("Game/config.json");*/
	
	//In case of error
	/*if (config_file != NULL)
	{*/
		//ret = true;

		/*config = json_value_get_object(config_file);
		app_config = json_object_dotget_object(config,"App");
		appName = json_object_dotget_string(app_config, "App Name");
		orgName = json_object_dotget_string(app_config, "Org Name");*/

		int cap = 60;
		if (cap > 0)
		{
			capped_ms = 1000 / cap;
		}
		//}
		//------------------------------------


		// Call Init() in all modules
		p2List_item<Module*>* item = list_modules.getFirst();

		while (item != NULL && ret == true)
		{
			if (item->data->IsEnabled())
				ret = item->data->Init();
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

	//}
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
	ms_index = (ms_index + 1) % IM_ARRAYSIZE(ms_log);


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

	item = list_modules.getFirst();
	ImGui_ImplSdlGL3_NewFrame(window->window);

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->Update(dt);
		item = item->next;
	}

	//CONFIG WINDOW -------------------------------------
	if (showconfig)
	{
		item = list_modules.getFirst();
		ImGui::Begin("CONFIGURATION");
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::Text("App Name:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3D Engine");
			ImGui::Text("Organization Name:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Elliot & Jordi S.A.");
			static int fps = 60;
			ImGui::SliderInt("Max FPS", &fps, 0, 60);
			ImGui::SameLine(); ShowHelpMarker("0 = no frame cap");
			ImGui::Text("Framerate:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f", fps_log[frame_index - 1]);
			ImGui::PlotHistogram("", fps_log, IM_ARRAYSIZE(App->fps_log), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
			ImGui::Text("Milliseconds:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f", ms_log[ms_index - 1]);
			ImGui::PlotHistogram("", ms_log, IM_ARRAYSIZE(App->ms_log), 0, NULL, 0.0f, 50.0f, ImVec2(0, 80));

		}
		while (item != NULL && ret == UPDATE_CONTINUE)
		{
			if (item->data->IsEnabled())
				ret = item->data->UpdateConfig(dt);
			item = item->next;
		}
		ImGui::End();
		//----------------------------------------------
	}
	ImGui::Render();

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PostUpdate(dt);
		item = item->next;
	}


	//----------------------------------------------------

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->CleanUp();
		item = item->prev;
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