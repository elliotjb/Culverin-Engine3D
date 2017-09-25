#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleHardware.h"
#include "ModuleObjects.h"
#include "imgui.h"
#include "Gl3W\include\glew.h"
#include "imgui_impl_sdl_gl3.h"
#include "Algorithm\Random\LCG.h"
#include "SDL/include/SDL.h"

ModuleGUI::ModuleGUI(bool start_enabled): Module(start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

static void ShowHelpMarker(const char* desc, const char* icon = "(?)")
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

bool ModuleGUI::Start()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	displaymode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
	int display_count = 0;
	if ((display_count = SDL_GetNumVideoDisplays()) < 1) {
		SDL_Log("SDL_GetNumVideoDisplays returned: %i", display_count);
		return 1;
	}

	if (SDL_GetDisplayMode(displayIndex, modeIndex, &displaymode) != 0) {
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
		return 1;
	}


	winManager.push_back(new Hardware(false));   //0---- HARDWARE
	winManager.push_back(new ModuleObjects(false)); //1---- WINDOW_OBJECTS
	winManager[1]->obj = (ModuleObjects*)winManager[1];




	//Capsule_A = (Capsule(float3(200, 0, 0), float3(200, 0, 3), 1));
	//Capsule_B = (Capsule(float3(0, -3, 0), float3(0, 3, 0), 1));
	//line = (Line(float3(0, 0, 0), float3(3, 1, 2)));
	//plane = (Plane(float3(3, 2, 1), float3(-3, 0, -2)));
	return true;
}

update_status ModuleGUI::Update(float dt)
{
	//IMGUI----------------------------------------------------
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	//ShowTest -----------------------
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		ShowTest = !ShowTest;
	}
	if (ShowTest)
	{
		ImGui::ShowTestWindow();
	}
	// bools statics ----------------------
	static bool window_create_objects = false;
	static bool window_show_objects = false;
	static bool window_Random_generator = false;
	static bool window_about_us = false;
	static bool configuration = false;



	// Main Menu --------------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl + N"))
			{

			}
			if (ImGui::MenuItem("Open Scene", "Ctrl + O"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save Scene", "Ctrl + S"))
			{

			}
			if (ImGui::MenuItem("Save Scene as..."))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl + Z"))
			{

			}
			if (ImGui::MenuItem("Redo", "Ctrl + Y"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl + X"))
			{

			}
			if (ImGui::MenuItem("Copy", "Ctrl + C"))
			{

			}
			if (ImGui::MenuItem("Paste", "Ctrl + V"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Etc..."))
			{

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Create Objects"))
			{
				//window_create_objects = !window_create_objects;
				winManager[CREATEOBJETCS]->obj->OpenClose_CreateObjects();
				winManager[CREATEOBJETCS]->OpenClose();
			}
			if (ImGui::MenuItem("Objects in Scene"))
			{
				winManager[CREATEOBJETCS]->obj->OpenClose_ShowObjects();
				winManager[CREATEOBJETCS]->OpenClose();
			}
			if (ImGui::MenuItem("Random Generator"))
			{
				window_Random_generator = !window_Random_generator;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Hardware"))
			{
				winManager[HARDWARE]->OpenClose();
				//App->hardware->OpenClose();
			}
			if (ImGui::MenuItem("Console", "º"))
			{
				App->console->OpenClose();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Configuration"))
			{
				configuration = !configuration;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About us..."))
			{
				window_about_us = !window_about_us;
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Documentation 'GitHub'"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download latest"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/releases/download/0.1.1/Release.-.3D.Engine.v0.1.1.zip", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/releases/download/0.1.1/Release.-.3D.Engine.v0.1.1.zip", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Readme"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/blob/master/README.md", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}


	//Machine Generator -------------------
	if (window_Random_generator)
	{
		static LCG random_generator;
		ImGui::Begin("Random Numbers Generator");
		ImGui::Spacing();
		ImGui::PushItemWidth(60);
		static int numbers_f = 0;
		static int numbers_i = 0;
		if (ImGui::Button("FLOAT"))
			numbers_f++;
		if (ImGui::Button("INT"))
			numbers_i++; ImGui::SameLine();

		static int n1 = 0;
		static int n2 = 0;
		ImGui::PushItemWidth(100);
		ImGui::InputInt("Min", &n1, 1, 0, 1); ImGui::SameLine();
		ImGui::InputInt("Max", &n2, 1, 0, 1);

		if (numbers_f & 1)
		{
			numbers_f++;
			floats_list.push_back(random_generator.Float());
			//ImGui::Text("%.4f", random_generator.Float());
		}

		if (numbers_i & 1)
		{
			if (n1 <= n2)
			{
				numbers_i++;
				ints_list.push_back(random_generator.Int(n1, n2));
			}
		}

		static int clear = 0;
		if (ImGui::Button("CLEAR"))
			clear++;
		if (clear & 1)
		{
			clear++;
			if (floats_list.size() > 0)
			{
				floats_list.clear();
			}

			if (ints_list.size() > 0)
			{
				ints_list.clear();
			}
		}

		ImGui::PopItemWidth();
		ImGui::Spacing();

		ImGui::Text("FLOATS --------------------");
		for (uint n = 0; n < floats_list.size(); n++)
		{
			ImGui::Text("%i - %.4f", n, floats_list[n]);
		}

		ImGui::Text("INTS --------------------");
		for (uint n = 0; n < ints_list.size(); n++)
		{
			ImGui::Text("%i - %i", n, ints_list[n]);
		}
		ImGui::End();
	}
	

	//----------------------------------------------
	// Window About Us... ---------------------------------
	if (window_about_us)
	{
		ImGui::Begin("About Us...");
		//Name of your Engine
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("3D Engine");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Desciption
		ImGui::Text("3D Engine");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Name of the Author
		ImGui::Text("Authors:");
		ImGui::Spacing();
		ImGui::Text("Elliot Jimenez Bosch");
		ImGui::Text("Jordi Ona Rufi");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Libraries
		ImGui::Text("Libraries:");
		ImGui::Spacing();
		ImGui::Text("SDL");

		ImGui::End();
	}

	//// Windows to Create Objects ----------------------
	//if (window_create_objects)
	//{
	//	ShowCreateObjects();
	//}

	//// Windows See Objects in Scene ----------------------
	//if (window_show_objects)
	//{
	//	ShowObjectsinScene();
	//}

	// Console --------------------------
	if (App->console->IsOpen())
	{
		ShowExampleAppConsole();
	}


	//Configuration Window ------------------------
	if (configuration)
	{
		ShowConfig();
	}

	//Update All Modules ----------------------------------
	UpdateWindows(dt);

	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	return true;
}

void ModuleGUI::UpdateWindows(float dt)
{
	std::vector<WindowManager*>::iterator window = winManager.begin();
	for (int i = 0; i < winManager.size(); i++)
	{
		if (window[i]->IsActive())
		{
			window[i]->Update(dt);
		}
	}
}

void ModuleGUI::ShowExampleAppConsole()
{
	App->console->Draw("CONSOLE");
}


void ModuleGUI::ShowHardware()
{

}

void ModuleGUI::ShowConfig()
{
	ImGui::Begin("CONFIGURATION");
	ImGui::Spacing();
	//ImGui::PushItemWidth(150);
	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Text("App Name:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3D Engine");
		ImGui::Text("Organization Name:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Elliot & Jordi S.A.");
		static int fps = 60;
		ImGui::SliderInt("Max FPS", &fps, 0, 60);
		ImGui::SameLine(); ShowHelpMarker("0 = no frame cap");

		ImGui::PlotHistogram("Framerate", App->fps_log, IM_ARRAYSIZE(App->fps_log), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
		ImGui::PlotHistogram("Millisecons Histogram", App->ms_log, IM_ARRAYSIZE(App->ms_log), 0, NULL, 0.0f, 60.0f, ImVec2(0, 80));

	}

	if (ImGui::CollapsingHeader("Window"))
	{
		static bool fullscreen = false;
		static bool resizable = false;
		static bool borderless = false;
		static bool full_desktop = false;
		static int brightness = 0;
		static int width = SCREEN_WIDTH * SCREEN_SIZE;
		static int height = SCREEN_HEIGHT * SCREEN_SIZE;
		static int refresh = displaymode.refresh_rate;

		ImGui::Text("Refresh rate:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", displaymode.refresh_rate);
		ImGui::SliderInt("Brightness", &brightness, 0, 100);
		ImGui::SliderInt("Width", &width, 0, 4096);
		ImGui::SliderInt("Height", &height, 0, 3072);

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
		{
			//App->window->SetFullscreeen(fullscreen);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
		{
			//App->window->SetResizable(resizable);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Restart to apply");
		}
		if (ImGui::Checkbox("Borderless", &fullscreen))
		{
			//App->window->SetBorderless(fullscreen);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &resizable))
		{
			//App->window->SetFullDesktop(resizable);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Restart to apply");
		}
	}

	if (ImGui::CollapsingHeader("Audio"))
	{
		static int volume = 50;
		ImGui::SliderInt("Volume", &volume, 0, 100);
	}

	if (ImGui::CollapsingHeader("Input"))
	{
	}

	ImGui::End();
}
