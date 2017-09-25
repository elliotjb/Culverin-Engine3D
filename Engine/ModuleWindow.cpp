#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL\include\SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
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
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
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
	if(window != NULL)
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
		static bool fullscreen = false;
		static bool resizable = true;
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
			//SetFullscreen(fullscreen);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
		{
			//SetResizable(resizable);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Restart to apply");
		}
		if (ImGui::Checkbox("Borderless", &borderless))
		{
			//SetBorderless(fullscreen);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop))
		{
			//SetFullDesktop(resizable);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Restart to apply");
		}
	}

	return UPDATE_CONTINUE;
}
