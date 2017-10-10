#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleHardware.h"
#include "ModuleGeometries.h"
#include "ModuleInspector.h"
#include "ModuleHierarchy.h"
#include "ModuleSceneWorld.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "ImGui\imgui_dock_v2.h"
#include "Gl3W\include\glew.h"
#include "Algorithm\Random\LCG.h"
#include "SDL\include\SDL.h"
#include "ModuleObjectsUI.h"

ModuleGUI::ModuleGUI(bool start_enabled): Module(start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Start()
{
	glewInit();
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	ImGui_ImplSdlGL3_Init(App->window->window);

	winManager.push_back(new Hardware());   //0---- HARDWARE
	winManager.push_back(new ModuleObjectsUI()); //1---- WINDOW_OBJECTS
	winManager.push_back(new Inspector()); //2---- INSPECTOR
	winManager.push_back(new Hierarchy()); //3---- Hierarchy
	winManager.push_back(new SceneWorld()); //4---- SceneWorld
	winManager[INSPECTOR]->active[0].active = true;
	winManager[HIERARCHY]->active[0].active = true;
	winManager[SCENEWORLD]->active[0].active = true;

	std::vector<WindowManager*>::iterator window = winManager.begin();
	for (int i = 0; i < winManager.size(); i++)
	{
		window[i]->Start();
	}

	LoadDocks();

	//Capsule_A = (Capsule(float3(200, 0, 0), float3(200, 0, 3), 1));
	//Capsule_B = (Capsule(float3(0, -3, 0), float3(0, 3, 0), 1));
	//line = (Line(float3(0, 0, 0), float3(3, 1, 2)));
	//plane = (Plane(float3(3, 2, 1), float3(-3, 0, -2)));
	return true;
}

update_status ModuleGUI::Update(float dt)
{
	perf_timer.Start();

	//ShowTest -----------------------
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		ShowTest = !ShowTest;
	}
	if (ShowTest)
	{
		ImGui::ShowTestWindow();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		dock_test = !dock_test;
	}


	// bools statics ----------------------
	static bool window_create_objects = false;
	static bool window_show_objects = false;
	static bool window_Random_generator = false;
	static bool window_about_us = false;
	static bool configuration = false;
	static bool window_infoMouse = false;
	static bool window_exit = false;

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
			if (ImGui::MenuItem("Exit", NULL, &window_exit))
			{
				if (isSaved)
				{
					ImGui::EndMenu();
					ImGui::EndMainMenuBar();
					return UPDATE_STOP;
				}
				else
				{
					reposition_exit = true;
				}
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
			if (ImGui::MenuItem("Look View to Selected", "F"))
			{

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty", "Ctrl + Shift + N"))
			{

			}
			ImGui::Separator();
			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					winManager[CREATEOBJETCS]->SpecialFunction("cube");
				}

				if (ImGui::MenuItem("Sphere"))
				{
					winManager[CREATEOBJETCS]->SpecialFunction("sphere");
				}
				if (ImGui::MenuItem("Capsule"))
				{

				}
				if (ImGui::MenuItem("Cylinder"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Audio"))
			{

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Inspector"))
			{
				winManager[INSPECTOR]->active[0].OpenClose();
			}
			if (ImGui::MenuItem("Create Objects"))
			{
				winManager[CREATEOBJETCS]->active[CREATE].OpenClose();
			}
			if (ImGui::MenuItem("Objects in Scene"))
			{
				winManager[CREATEOBJETCS]->active[SHOW].OpenClose();
			}
			if (ImGui::MenuItem("Random Generator"))
			{
				window_Random_generator = !window_Random_generator;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Hardware"))
			{
				winManager[HARDWARE]->active[0].OpenClose();
			}
			if (ImGui::MenuItem("Console", "º"))
			{
				App->console->OpenClose();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Property editor", NULL, &window_infoMouse))
			{

			}
			if (ImGui::MenuItem("Configuration"))
			{
				App->showconfig = !App->showconfig;
			}
			if (ImGui::MenuItem("Performance"))
			{
				App->showperformance = !App->showperformance;
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
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/issues/new", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("README"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/elliotjb/3D-Engine/blob/master/README.md", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
		menu_height = ImGui::GetWindowSize().y;

		ImGui::EndMainMenuBar();
	}

	if (dock_test)
	{
		DockTest();
	}

	//Machine Generator -------------------
	if (window_Random_generator)
	{
		static LCG random_generator;
		if (!ImGui::Begin("Random Numbers Generator", &window_Random_generator, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return UPDATE_CONTINUE;
		}
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
			//ImGui::Text("%.4f", random_generator.Float())
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
		if (!ImGui::Begin("About Us...", &window_about_us, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return UPDATE_CONTINUE;
		}
		//Name of your Engine
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "CULVERIN Engine");


		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Description
		ImGui::Text("This is a project consistent of creating a Game Engine,\nthat allows you to use tools to create video games");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Name of the Author
		ImGui::Text("Authors:");
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Elliot Jimenez Bosch");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Jordi Ona Rufi");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// Libraries
		ImGui::Text("Libraries:");
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SDL");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ImGui");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MathGeoLib");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Parson");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "OpenGL");

		ImGui::End();
	}

	//// Windows to Create Objects ----------------------
	//if (window_create_objects)
	//{
	//	ShowCreateObjects();
	//}

	// Windows EXIT ---------------------------------
	if (window_exit)
	{
		ShowWindowExit(&window_exit);
		if (gameSave)
		{
			return UPDATE_STOP;
		}
	}

	// ----------------------------------------------

	if (window_infoMouse)
	{
		ShowInfoMouse(&window_infoMouse);
	}

	// Console --------------------------
	if (App->console->IsOpen())
	{
		//ShowExampleAppConsole();
	}

	//Update All Modules ----------------------------------
	UpdateWindows(dt);

	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

void ModuleGUI::LoadDocks()
{
	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("ImGuiDock.json");

	config = json_value_get_object(config_file);
	config_node = json_object_get_object(config, "Docking");
	getDockContext()->LoadDock(config_node);
}

bool ModuleGUI::CleanUp()
{
	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("ImGuiDock.json"); 

	config = json_value_get_object(config_file);
	config_node = json_object_get_object(config, "Docking");
	getDockContext()->SaveDock(config_node);

	json_serialize_to_file(config_file, "ImGuiDock.json");


	return true;
}

void ModuleGUI::ShowPerformance(int ms_index)
{
	if (ImGui::CollapsingHeader("GUI"))
	{
		ImGui::Text("Pre-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", pre_log[ms_index - 1]);
		ImGui::PlotHistogram("", pre_log, IM_ARRAYSIZE(pre_log), 0, NULL, 0.0f, 5.0f, ImVec2(0, 80));
		ImGui::Text("Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", up_log[ms_index - 1]);
		ImGui::PlotHistogram("", up_log, IM_ARRAYSIZE(up_log), 0, NULL, 0.0f, 5.0f, ImVec2(0, 80));
		ImGui::Text("Post-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", post_log[ms_index - 1]);
		ImGui::PlotHistogram("", post_log, IM_ARRAYSIZE(post_log), 0, NULL, 0.0f, 5.0f, ImVec2(0, 80));
	}
}

void ModuleGUI::DockTest()
{

	static bool show_scene1 = true;
	static bool show_scene2 = true;
	static bool show_scene3 = true;
	static bool show_scene4 = true;
	static bool show_scene5 = true;
	static bool show_scene6 = true;

	//DOCKING TEST -----------------------------------------------------------
	/*if (ImGui::GetIO().DisplaySize.y > 0) 
	{
		////////////////////////////////////////////////////
		// Setup root docking window                      //
		// taking into account menu height and status bar //
		////////////////////////////////////////////////////
		auto pos = ImVec2(0, menu_height);
		auto size = ImGui::GetIO().DisplaySize;
		size.y -= pos.y;
		ImGui::DockContext::rootDock(pos, ImVec2(size.x, size.y - 25.0f));

		// Draw status bar (no docking)
		ImGui::SetNextWindowSize(ImVec2(size.x, 25.0f), ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(ImVec2(0, size.y - 6.0f), ImGuiSetCond_Always);
		ImGui::Begin("statusbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize);
		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
		ImGui::End();
	}
	ImGui::Separator();
	// Draw docking windows
	if (ImGui::BeginDock("Docks", &show_scene1)) {
		ImGui::Print(); // print docking information
	}
	ImGui::EndDock();
	if (ImGui::BeginDock("Dummy1", &show_scene2)) {
		ImGui::Text("Placeholder!");
	}
	ImGui::EndDock();
	if (ImGui::BeginDock("Dummy2", &show_scene3)) {
		ImGui::Text("Placeholder!");
	}
	ImGui::EndDock();*/
	//-------------------------------------------------------
}

void ModuleGUI::UpdateWindows(float dt)
{
	static bool show_scene3 = true;
	//DockContext::Dock::setPosSize(ImVec2(0, 0), ImVec2(100,100));
	static int width;
	static int height;
	SDL_GetWindowSize(App->window->window, &width, &height);
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(width, height - 20));
	

	ImGui::Begin("MasterWindow", &show_scene3, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

	if (1)
	{
		if (1);
	}

	static GLuint icon_play = App->textures->LoadTexture("Imatges/UI/icon_play.png");
	static GLuint icon_pause = App->textures->LoadTexture("Imatges/UI/icon_pause.png");
	static GLuint icon_stop = App->textures->LoadTexture("Imatges/UI/icon_stop.png");
	//ImTextureID temp = play;
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(150,150,150,0));
	//ImGui::Image((ImTextureID*)play, ImVec2(300, 60));
	static ImVec2 pos_icon(20, 20);
	ImGui::SameLine(width / 2 - 40);
	ImGui::ImageButton((ImTextureID*)icon_play, pos_icon);
	ImGui::SameLine(width / 2 - 10);
	ImGui::ImageButton((ImTextureID*)icon_pause, pos_icon);
	ImGui::SameLine(width / 2 + 20);
	ImGui::ImageButton((ImTextureID*)icon_stop, pos_icon);
	ImGui::PopStyleColor(1);


	BeginWorkspace();

	//ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() - App->window->GetWidth() / 4, 23), 2);
	//ImGui::SetNextWindowSize(ImVec2(App->window->GetWidth() / 4, App->window->GetHeight() - 23), 2);
	/*BeginDock("TEST", &show_scene3, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Text("HELLO");
	EndDock();*/


	std::vector<WindowManager*>::iterator window = winManager.begin();
	for (int i = 0; i < winManager.size(); i++)
	{
		/*if (window[i]->IsActive())
		{
			window[i]->Update(dt);
		}*/
		window[i]->Update(dt);
	}

	if (App->console->IsOpen())
	{
		ShowExampleAppConsole();
	}

	EndWorkspace();
	ImGui::End();
}

void ModuleGUI::ShowExampleAppConsole()
{
	App->console->Draw("CONSOLE");
}


void ModuleGUI::ShowHardware()
{

}

void ModuleGUI::ShowInfoMouse(bool* active)
{
	ImGui::SetNextWindowPos(ImVec2(10, 18));
	if (!ImGui::Begin("Info", active, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	ImGui::Text("FPS: %.0f", App->fps_log[App->frame_index - 1]);
	ImGui::End();
}

void ModuleGUI::ShowWindowExit(bool* active)
{
	static int width = 0;
	static int height = 0;

	if (reposition_exit)
	{
		SDL_GetWindowSize(App->window->window, &width, &height);
		ImGui::SetNextWindowPos(ImVec2(width / 2 - 180, height / 2 - 80));
		reposition_exit = false;
	}
	if (!ImGui::Begin("Scene(s) Have Been Modified", active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("Do you want to save the changes...");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Your changes wiil be lost if you don't save them.");
	ImGui::Spacing();
	if (ImGui::Button("Save"))
	{
		reposition_exit = true;
		isSaved = true;
		gameSave = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Don't Save"))
	{
		reposition_exit = true;
		isSaved = true;
		gameSave = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		reposition_exit = true;
		*active = false;
	}
	if (*active == false)
	{
		reposition_exit = true;
	}
	ImGui::End();
}

//void ModuleGUI::ShowConfig()
//{
//	ImGui::Begin("CONFIGURATION");
//	ImGui::Spacing();
//	//ImGui::PushItemWidth(150);
//	if (ImGui::CollapsingHeader("Application"))
//	{
//		ImGui::Text("App Name:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3D Engine");
//		ImGui::Text("Organization Name:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Elliot & Jordi S.A.");
//		static int fps = 60;
//		ImGui::SliderInt("Max FPS", &fps, 0, 60);
//		ImGui::SameLine(); App->ShowHelpMarker("0 = no frame cap");
//		ImGui::Text("Framerate:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f", App->fps_log[App->frame_index-1]);
//		ImGui::PlotHistogram("", App->fps_log, IM_ARRAYSIZE(App->fps_log), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
//		ImGui::Text("Milliseconds:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f", App->ms_log[App->ms_index-1]);
//		ImGui::PlotHistogram("", App->ms_log, IM_ARRAYSIZE(App->ms_log), 0, NULL, 0.0f, 50.0f, ImVec2(0, 80));
//
//	}
//
//	if (ImGui::CollapsingHeader("Window"))
//	{
//		static bool fullscreen = false;
//		static bool resizable = true;
//		static bool borderless = false;
//		static bool full_desktop = false;
//		static int brightness = 0;
//		static int width = SCREEN_WIDTH * SCREEN_SIZE;
//		static int height = SCREEN_HEIGHT * SCREEN_SIZE;
//		static int refresh = displaymode.refresh_rate;
//
//		ImGui::Text("Refresh rate:"); ImGui::SameLine();
//		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", displaymode.refresh_rate);
//		ImGui::SliderInt("Brightness", &brightness, 0, 100);
//		ImGui::SliderInt("Width", &width, 0, 4096);
//		ImGui::SliderInt("Height", &height, 0, 3072);
//
//		if (ImGui::Checkbox("Fullscreen", &fullscreen))
//		{
//			//App->window->SetFullscreeen(fullscreen);
//		}
//		ImGui::SameLine();
//		if (ImGui::Checkbox("Resizable", &resizable))
//		{
//			//App->window->SetResizable(resizable);
//		}
//		if (ImGui::IsItemHovered())
//		{
//			ImGui::SetTooltip("Restart to apply");
//		}
//		if (ImGui::Checkbox("Borderless", &borderless))
//		{
//			//App->window->SetBorderless(fullscreen);
//		}
//		ImGui::SameLine();
//		if (ImGui::Checkbox("Full Desktop", &full_desktop))
//		{
//			//App->window->SetFullDesktop(resizable);
//		}
//		if (ImGui::IsItemHovered())
//		{
//			ImGui::SetTooltip("Restart to apply");
//		}
//	}
//
//	if (ImGui::CollapsingHeader("Audio"))
//	{
//		static int volume = 50;
//		ImGui::SliderInt("Volume", &volume, 0, 100);
//	}
//
//	//if (ImGui::CollapsingHeader("Input"))
//	//{
//	//}
//
//	ImGui::End();
//}
