#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ImGui\imgui.h"
#include "MathGeoLib.h"

class _Cube;

class Scene : public Module
{
public:
	Scene(bool start_enabled = false);
	virtual ~Scene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowPerformance(int ms_index);

public:
	GLuint ImageName;
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	_Cube* test;

};