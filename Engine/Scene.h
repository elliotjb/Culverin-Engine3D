#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ImGui/imgui.h"
#include "MathGeoLib.h"

class _Cube;
class GameObject;

class Scene : public Module
{
public:
	Scene(bool start_enabled = false);
	virtual ~Scene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowPerformance(int ms_index);

	GameObject* CreateGameObject();

public:
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	_Cube* test;

	GLuint tex;
	GLuint Lenna;

	GameObject* rootObject = nullptr;
};