#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL\include\SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "parson.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "Window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_Object* node)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = json_object_get_number(node, "Width");
		height = json_object_get_number(node, "Height");
		brightness = json_object_get_number(node, "Brightness");
		scale = json_object_get_number(node, "Scale");
		
		if (fullscreen = json_object_get_boolean(node, "Fullscreen"))
		{
			selected = 0;
		}
		if (fullscreen_d = json_object_get_boolean(node, "Full Desktop"))
		{
			selected = 1;
		}
		if (resizable = json_object_get_boolean(node, "Resizable"))
		{
			selected = 2;
		}
		if (borderless = json_object_get_boolean(node, "Borderless"))
		{
			selected = 3;
		}


		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

//Use OpenGL 2.1
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

if (fullscreen == true)
{
	flags |= SDL_WINDOW_FULLSCREEN;
}

if (resizable == true)
{
	flags |= SDL_WINDOW_RESIZABLE;
}

if (WIN_OPENGL == true)
{
	flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
}

if (borderless == true)
{
	flags |= SDL_WINDOW_BORDERLESS;
}

if (fullscreen_d == true)
{
	flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
}

window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

if (window == NULL)
{
	LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	ret = false;
}
else
{
	//Get window surface
	screen_surface = SDL_GetWindowSurface(window);
}

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
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
		ImGui_ImplSdlGL3_Shutdown();
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

update_status ModuleWindow::UpdateConfig(float dt)
{
	if (ImGui::CollapsingHeader("Window"))
	{
		static int refresh = displaymode.refresh_rate;

		ImGui::Text("Refresh rate:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", displaymode.refresh_rate);
		ImGui::SliderInt("Brightness", &brightness, 0, 100); ImGui::SameLine();
		if (ImGui::Button("APPLY##brightness"))
		{
			float value = (float)brightness * 0.01;
			SDL_SetWindowBrightness(window, value);
		}
		ImGui::SliderInt("Width", &width, 0, 4096);
		ImGui::SliderInt("Height", &height, 0, 3072);
		if (ImGui::Button("APPLY##win_size"))
		{
			SDL_SetWindowSize(window, width, height);
		}
		const char* win_opt[] = { "Fullscreen", "Full Desktop", "Windowed" };
		ImGui::Combo("WINDOW OPTIONS", &selected, win_opt, IM_ARRAYSIZE(win_opt)); ImGui::SameLine();
		if (ImGui::Button("APPLY##win_options"))
		{
			SetWindowOption(selected);
		}
		if (ImGui::Checkbox("Borderless", &borderless))
		{
			if (borderless)
			{
				SDL_SetWindowBordered(window, SDL_FALSE);
			}
			else
			{
				SDL_SetWindowBordered(window, SDL_TRUE);
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleWindow::SetWindowOption(int i)
{
	switch (i)
	{
	case 0:
	{
		//SetFullscreen -------------
		fullscreen = true;
		fullscreen_d = false;
		resizable = false;
		borderless = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		break;
	}
	case 1:
	{
		//SetFullDesktop -------------
		fullscreen_d = true;
		fullscreen = false;
		resizable = false;
		borderless = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

		break;
	}
	case 2:
	{
		//SetResizable ----------
		resizable = true;
		fullscreen_d = false;
		fullscreen = false;
		borderless = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_RESIZABLE);
		break;
	}
	default:
	{
		break;
	}
	}
}

uint ModuleWindow::GetScale() const
{
	return scale;
}

uint ModuleWindow::GetWidth() const
{
	return width;
}

uint ModuleWindow::GetHeight() const
{
	return height;
}