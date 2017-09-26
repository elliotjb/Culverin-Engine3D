#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "parson.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSON_Object* node);
	bool CleanUp();

	void SetTitle(const char* title);

	update_status UpdateConfig(float dt);

	void SetWindowOption(int i);
	void SetWindowRes(int i);

	// Retrieve window scale
	uint GetScale() const;
	uint GetWidth() const;
	uint GetHeight() const;


public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	SDL_DisplayMode displaymode;
	int displayIndex = 0;
	int modeIndex = 0;

	//Window Parameters
	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;
	int scale = SCREEN_SIZE;
	int brightness = 100;

	int selected_op = -1;
	int selected_res = -1;

	bool fullscreen = false;
	bool fullscreen_d = false;
	bool resizable = false;
	
	bool borderless = false;

	//Uint32 flags;

};

#endif // __ModuleWindow_H__