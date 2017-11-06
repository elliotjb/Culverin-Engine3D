#pragma once

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"

class WindowGame : public WindowManager
{
public:
	WindowGame();
	virtual ~WindowGame();

	bool Start();
	update_status Update(float dt);
	void SetTexture();
	void ShowSceneWorld();
	bool CleanUp();
};

