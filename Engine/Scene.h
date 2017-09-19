#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "imgui.h"
#include "MathGeoLib.h"

class Scene : public Module
{
public:
	Scene(Application* app, bool start_enabled = false);
	virtual ~Scene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	//test
	Sphere Sphere_A;
	Sphere Sphere_B;
	Capsule Capsule_A;
	Capsule Capsule_B;
	Line line;
	Plane plane;


};