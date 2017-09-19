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

	p2List<Sphere> list_Sphere;
	p2List<Capsule> list_Capsule;
	p2List<Plane> list_Plane;
	p2List<Ray> list_Ray;

	//test
	Sphere Sphere_A;
	Sphere Sphere_B;
	Capsule Capsule_A;
	Capsule Capsule_B;
	Line line;
	Plane plane;
	bool ShowTest = false;

};